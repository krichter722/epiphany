/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2; -*- */
/* vim: set sw=2 ts=2 sts=2 et: */
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
 */

#include "config.h"
#include "ephy-embed-shell.h"
#include "ephy-frecent-store.h"
#include "ephy-snapshot-service.h"
#include "ephy-history-service.h"
#include "ephy-overview.h"

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#define EPHY_OVERVIEW_GET_PRIVATE(object) (G_TYPE_INSTANCE_GET_PRIVATE ((object), EPHY_TYPE_OVERVIEW, EphyOverviewPrivate))

enum
{
  PROP_0,
};

struct _EphyOverviewPrivate
{
  GtkWidget *frecent_view;
};

G_DEFINE_TYPE (EphyOverview, ephy_overview, GTK_TYPE_GRID)

static void
ephy_overview_set_property (GObject *object,
                            guint prop_id,
                            const GValue *value,
                            GParamSpec *pspec)
{
  EphyOverview *overview = EPHY_OVERVIEW (object);

  switch (prop_id)
  {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
ephy_overview_get_property (GObject *object,
                            guint prop_id,
                            GValue *value,
                            GParamSpec *pspec)
{
  EphyOverview *overview = EPHY_OVERVIEW (object);

  switch (prop_id)
  {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

#if 0
static gboolean
active_view_item_deleted (GtkWidget *widget,
                          const gchar *path,
                          EphyOverview *overview)

{
  GtkTreeModel *model;
  GtkTreeIter iter;
  EphyNotebook *notebook;
  GtkWidget* embed;
  GtkTreePath* tree_path;
  guint position;

  model = gd_main_view_get_model (GD_MAIN_VIEW (widget));
  notebook = ephy_active_store_get_notebook (EPHY_ACTIVE_STORE (model));
  tree_path = gtk_tree_path_new_from_string (path);
  gtk_tree_model_get_iter (model, &iter, tree_path);
  gtk_tree_model_get (model, &iter,
                      EPHY_ACTIVE_STORE_TAB_POS, &position,
                      -1);
  embed = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), position);
  g_signal_emit_by_name (notebook, "tab-close-request", embed);
  gtk_tree_path_free (tree_path);

  return TRUE;
}
#endif

static gboolean
frecent_view_item_deleted (GtkWidget *widget,
                           gchar *path,
                           gpointer data)
{
  EphyFrecentStore *store;
  GtkTreeIter iter;
  GtkTreePath *tree_path;

  store = EPHY_FRECENT_STORE (gd_main_view_get_model (GD_MAIN_VIEW (widget)));
  tree_path = gtk_tree_path_new_from_string (path);
  gtk_tree_model_get_iter (GTK_TREE_MODEL (store), &iter, tree_path);
  ephy_frecent_store_set_hidden (store, &iter);
  gtk_tree_path_free (tree_path);

  return TRUE;
}

static void
main_view_item_activated (GtkWidget *widget,
                          gchar *id,
                          GtkTreePath *path,
                          EphyOverview *overview)
{
  guint position;
  GtkTreeModel *model;
  GtkTreeIter iter;
  char *url;

  model = gd_main_view_get_model (GD_MAIN_VIEW (widget));
  gtk_tree_model_get_iter (model, &iter, path);
#if 0
  if (EPHY_IS_ACTIVE_STORE (model)) {
    gtk_tree_model_get (model, &iter,
                        EPHY_ACTIVE_STORE_TAB_POS, &position,
                        -1);
    gtk_notebook_set_current_page (GTK_NOTEBOOK (ephy_window_get_notebook (window)),
                                   position);
    ephy_window_set_overview_mode (window, FALSE);
  } else {
#endif
    gtk_tree_model_get (model, &iter,
                        EPHY_OVERVIEW_STORE_URI, &url,
                        -1);
    g_signal_emit_by_name (overview, "open-link", url);
#if 0
    ephy_shell_new_tab (ephy_shell, window, NULL, url,
                        EPHY_NEW_TAB_OPEN_PAGE |
                        EPHY_NEW_TAB_JUMP |
                        EPHY_NEW_TAB_IN_EXISTING_WINDOW);
#endif
    g_free (url);
#if 0
  }
#endif
}

static GdkPixbuf *
ephy_overview_get_icon (const gchar *icon_name)
{
	GError *error = NULL;
	GtkIconTheme *icon_theme;
	GdkPixbuf *pixbuf;

	icon_theme = gtk_icon_theme_get_default ();

	pixbuf = gtk_icon_theme_load_icon (icon_theme,
                                     icon_name,
                                     EPHY_THUMBNAIL_WIDTH,
                                     0,
                                     &error);

	if (!pixbuf) {
		g_warning ("Couldn't load icon: %s", error->message);
		g_error_free (error);
	}

	return pixbuf;
}

static void
ephy_overview_constructed (GObject *object)
{
  EphyOverviewStore *store;
  EphyOverview *self = EPHY_OVERVIEW (object);
  GdkPixbuf *default_icon;

  if (G_OBJECT_CLASS (ephy_overview_parent_class)->constructed)
    G_OBJECT_CLASS (ephy_overview_parent_class)->constructed (object);

  default_icon = ephy_overview_get_icon ("text-html");

  self->priv->frecent_view = GTK_WIDGET (gd_main_view_new (GD_MAIN_VIEW_ICON));
  g_signal_connect (self->priv->frecent_view, "item-activated",
                    G_CALLBACK (main_view_item_activated), object);
  g_signal_connect (self->priv->frecent_view, "item-deleted",
                    G_CALLBACK (frecent_view_item_deleted), NULL);

  store = ephy_embed_shell_get_frecent_store (EPHY_EMBED_SHELL (ephy_shell_get_default()));
  g_object_set (G_OBJECT (store),
                "default-icon", default_icon,
                NULL);
  gd_main_view_set_model (GD_MAIN_VIEW (self->priv->frecent_view),
                          GTK_TREE_MODEL (store));
  gtk_grid_attach (GTK_GRID (self), self->priv->frecent_view,
                   0, 0, 1, 1);
  gtk_widget_set_vexpand (self->priv->frecent_view, TRUE);
  gtk_widget_set_size_request (self->priv->frecent_view, -1, 320);

#if 0
  self->priv->active_view = GTK_WIDGET (gd_main_view_new (GD_MAIN_VIEW_ICON));
  g_signal_connect (self->priv->active_view, "item-activated",
                    G_CALLBACK (main_view_item_activated),
                    self->priv->parent_window);
  g_signal_connect (self->priv->active_view, "item-deleted",
                    G_CALLBACK (active_view_item_deleted), self);
  notebook = EPHY_NOTEBOOK (ephy_window_get_notebook (EPHY_WINDOW (self->priv->parent_window)));
  store = EPHY_OVERVIEW_STORE (ephy_active_store_new (notebook));
  g_object_set (G_OBJECT (store),
                "history-service", service,
                "default-icon", default_icon,
                NULL);
  gd_main_view_set_model (GD_MAIN_VIEW (self->priv->active_view),
                          GTK_TREE_MODEL (store));
  gtk_grid_attach (GTK_GRID (self), self->priv->active_view,
                   0, 1, 1, 1);
#endif

  gtk_widget_show_all (GTK_WIDGET (self));
  g_object_unref (default_icon);
}

static void
ephy_overview_class_init (EphyOverviewClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = ephy_overview_set_property;
  object_class->get_property = ephy_overview_get_property;
  object_class->constructed  = ephy_overview_constructed;

  g_signal_new ("open-link",
                EPHY_TYPE_OVERVIEW,
                G_SIGNAL_RUN_LAST,
                0, NULL, NULL,
                g_cclosure_marshal_generic,
                G_TYPE_NONE,
                1,
                G_TYPE_STRING);

  g_type_class_add_private (object_class, sizeof (EphyOverviewPrivate));
}

static void
ephy_overview_init (EphyOverview *self)
{
  self->priv = EPHY_OVERVIEW_GET_PRIVATE (self);
  gtk_orientable_set_orientation (GTK_ORIENTABLE (self),
                                  GTK_ORIENTATION_VERTICAL);
}

GtkWidget *
ephy_overview_new (void)
{
  return g_object_new (EPHY_TYPE_OVERVIEW,
                       NULL);
}
