/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 *  Copyright © 2012 Igalia S.L.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Author: Claudio Saavedra  <csaavedra@igalia.com>
 */

#include "config.h"
#include "ephy-snapshot-service.h"

#ifndef GNOME_DESKTOP_USE_UNSTABLE_API
#define GNOME_DESKTOP_USE_UNSTABLE_API
#endif
#include <libgnome-desktop/gnome-desktop-thumbnail.h>

#include <webkit/webkit.h>


G_DEFINE_TYPE (EphySnapshotService, ephy_snapshot_service, G_TYPE_OBJECT)

#define SNAPSHOT_SERVICE_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), EPHY_TYPE_SNAPSHOT_SERVICE, EphySnapshotServicePrivate))

struct _EphySnapshotServicePrivate
{
  GnomeDesktopThumbnailFactory *factory;
};

static void
ephy_snapshot_service_class_init (EphySnapshotServiceClass *klass)
{
  g_type_class_add_private (klass, sizeof (EphySnapshotServicePrivate));
}

static void
ephy_snapshot_service_init (EphySnapshotService *self)
{
  self->priv = SNAPSHOT_SERVICE_PRIVATE (self);

  self->priv->factory = gnome_desktop_thumbnail_factory_new (GNOME_DESKTOP_THUMBNAIL_SIZE_LARGE);
}

/**
 * ephy_snapshot_service_get_default:
 *
 * Gets the default instance of #EphySnapshotService.
 *
 * Returns: a #EphySnapshotService
 **/
EphySnapshotService *
ephy_snapshot_service_get_default (void)
{
  static EphySnapshotService *service = NULL;

  if (service == NULL)
    service = g_object_new (EPHY_TYPE_SNAPSHOT_SERVICE, NULL);

  return service;
}

typedef struct {
  char *url;
  time_t mtime;
  EphySnapshotServiceCallback callback;
  gpointer user_data;
  GtkWidget *window;
  GtkWidget *webview;
} SnapshotRequest;

static void
request_free (SnapshotRequest *request)
{
  g_free (request->url);

  if (request->window)
    gtk_widget_destroy (request->window);

  g_slice_free (SnapshotRequest, request);
}

static void
finish_request (SnapshotRequest *request, GdkPixbuf *snapshot)
{
  if (request->callback)
    request->callback (snapshot, request->user_data);

  request_free (request);
}

static void
webview_load_status_changed (WebKitWebView *webview,
			     GParamSpec *pspec,
			     SnapshotRequest *request)
{
  WebKitLoadStatus status;
  GdkPixbuf *snapshot, *scaled;
  EphySnapshotService *service;

  status = webkit_web_view_get_load_status (webview);

  if (status == WEBKIT_LOAD_FINISHED) {
    service = ephy_snapshot_service_get_default ();
    snapshot = gtk_offscreen_window_get_pixbuf (GTK_OFFSCREEN_WINDOW (request->window));
    scaled = gnome_desktop_thumbnail_scale_down_pixbuf (snapshot, 250, 125);
    gnome_desktop_thumbnail_factory_save_thumbnail (service->priv->factory,
						    scaled,
						    request->url,
						    request->mtime);
    g_object_unref (snapshot);
    finish_request (request, scaled);
  }
}

static GdkPixbuf *
ephy_snapshot_service_get_snapshot_from_cache (EphySnapshotService *service,
					       char *url, time_t mtime)
{
  GdkPixbuf *snapshot;
  char *uri;

  uri =  gnome_desktop_thumbnail_factory_lookup (service->priv->factory,
						 url, mtime);
  if (uri == NULL)
    return NULL;

  snapshot = gdk_pixbuf_new_from_file (uri, NULL);
  g_free (uri);

  return snapshot;
}

static gboolean
process_request (SnapshotRequest *request)
{
  EphySnapshotService *service;
  GdkPixbuf *snapshot;

  service = ephy_snapshot_service_get_default ();
  snapshot = ephy_snapshot_service_get_snapshot_from_cache (service,
							    request->url,
							    request->mtime);
  if (snapshot) {
    finish_request (request, snapshot);
  } else {
    GtkWidget *sw;
    request->window = gtk_offscreen_window_new ();
    request->webview = webkit_web_view_new ();
    sw = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
				    GTK_POLICY_NEVER, GTK_POLICY_NEVER);
    gtk_widget_set_size_request (sw, 800, 400);

    g_signal_connect (request->webview, "notify::load-status",
		      G_CALLBACK (webview_load_status_changed),
		      request);
    gtk_container_add (GTK_CONTAINER (request->window), sw);
    gtk_container_add (GTK_CONTAINER (sw), request->webview);
    gtk_widget_show_all (request->window);
    webkit_web_view_load_uri (WEBKIT_WEB_VIEW (request->webview), request->url);
  }

  return FALSE;
}

/**
 * ephy_snapshot_service_get_snapshot:
 * @service: a #EphySnapshotService
 * @url: the URL for which a snapshot is needed
 * @mtime: @the last
 * @callback: a #EphySnapshotServiceCallback
 * @userdata: user data to pass to @callback
 *
 * Schedules a query for a snapshot of @url. If there is an up-to-date
 * snapshot in the cache, this will be retrieved. Otherwise, this
 * the snapshot will be taken, cached, and retrieved.
 *
 **/
void
ephy_snapshot_service_get_snapshot (EphySnapshotService *service,
                                    const char *url,
				    const time_t mtime,
                                    EphySnapshotServiceCallback callback,
                                    gpointer userdata)
{
  SnapshotRequest *request;

  g_return_if_fail (EPHY_IS_SNAPSHOT_SERVICE (service));
  g_return_if_fail (url != NULL);

  request = g_slice_alloc0 (sizeof(SnapshotRequest));

  request->url = g_strdup (url);
  request->callback = callback;
  request->user_data = userdata;
  request->mtime = mtime;

  g_idle_add ((GSourceFunc) process_request, request);
}
