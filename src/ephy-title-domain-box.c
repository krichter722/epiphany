/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 *  02110-1301, USA.
 *
 */

#include "config.h"
#include "ephy-title-domain-box.h"

#include <libsoup/soup.h>

struct _EphyTitleDomainBoxPrivate {
  GtkWidget *title;
  GtkWidget *domain;

  EphyWebView *view;

  GBinding *title_binding;
  GBinding *domain_binding;
};

enum {
  PROP_0,

  PROP_TITLE,
  PROP_DOMAIN,
  PROP_VIEW
};

#define EPHY_TITLE_DOMAIN_BOX_GET_PRIVATE(object) (G_TYPE_INSTANCE_GET_PRIVATE ((object), EPHY_TYPE_TITLE_DOMAIN_BOX, EphyTitleDomainBoxPrivate))

G_DEFINE_TYPE (EphyTitleDomainBox, ephy_title_domain_box, GTK_TYPE_BOX)

static gboolean
transform_uri_to_domain (GBinding *binding,
                         const GValue *source_value,
                         GValue *target_value,
                         gpointer user_data)
{
  const char *uri;
  SoupURI *soup_uri;

  uri = g_value_get_string (source_value);
  if (!uri)
    return FALSE;

  soup_uri = soup_uri_new (uri);
  g_value_set_string (target_value, soup_uri->host);
  soup_uri_free (soup_uri);

  return TRUE;
}

static void
reset_bindings (EphyTitleDomainBox *box)
{
  EphyTitleDomainBoxPrivate *priv = box->priv;

  g_clear_object (&priv->title_binding);
  
  if (priv->view)
    priv->title_binding = g_object_bind_property (priv->view, "title",
                                                  box, "title",
                                                  G_BINDING_SYNC_CREATE);

  g_clear_object (&priv->domain_binding);

  if (priv->view)
    priv->domain_binding = g_object_bind_property_full (priv->view, "uri",
                                                        box, "domain",
                                                        G_BINDING_SYNC_CREATE,
                                                        transform_uri_to_domain,
                                                        NULL, NULL, NULL);
}

static void
ephy_title_domain_box_constructed (GObject *object)
{
  reset_bindings (EPHY_TITLE_DOMAIN_BOX (object));
  
  G_OBJECT_CLASS (ephy_title_domain_box_parent_class)->constructed (object);
}

static void
ephy_title_domain_box_get_property (GObject *object,
                                    guint prop_id,
                                    GValue *value,
                                    GParamSpec *pspec)
{
  EphyTitleDomainBoxPrivate *priv = EPHY_TITLE_DOMAIN_BOX (object)->priv;

  switch (prop_id) {
  case PROP_TITLE:
    break;
  case PROP_DOMAIN:
    break;
  case PROP_VIEW:
    g_value_set_object (value, priv->view);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
ephy_title_domain_box_set_property (GObject *object,
                                    guint prop_id,
                                    const GValue *value,
                                    GParamSpec *pspec)
{
  EphyTitleDomainBoxPrivate *priv = EPHY_TITLE_DOMAIN_BOX (object)->priv;

  switch (prop_id) {
  case PROP_TITLE: {
    char *markup = g_markup_printf_escaped ("<span weight='bold'>%s</span>", g_value_get_string (value));
    gtk_label_set_markup (GTK_LABEL (priv->title), markup);
    g_free (markup);
    break;
  }
  case PROP_DOMAIN: {
    const char *base_domain = soup_tld_get_base_domain (g_value_get_string (value), NULL);
    char *markup = g_markup_printf_escaped ("<span foreground='gray' size='small'>%s</span>", base_domain);
    gtk_label_set_markup (GTK_LABEL (priv->domain), markup);
    g_free (markup);
    break;
  }
  case PROP_VIEW:
    ephy_title_domain_box_set_view (EPHY_TITLE_DOMAIN_BOX (object),
                                    g_value_get_object (value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
ephy_title_domain_box_class_init (EphyTitleDomainBoxClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->constructed = ephy_title_domain_box_constructed;
  gobject_class->get_property = ephy_title_domain_box_get_property;
  gobject_class->set_property = ephy_title_domain_box_set_property;

  g_object_class_install_property (gobject_class,
                                   PROP_TITLE,
                                   g_param_spec_string ("title",
                                                        "Domain",
                                                        "The box's title",
                                                        "",
                                                        G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB));
  g_object_class_install_property (gobject_class,
                                   PROP_DOMAIN,
                                   g_param_spec_string ("domain",
                                                        "Domain",
                                                        "The box's domain",
                                                        "",
                                                        G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB));

  g_object_class_install_property (gobject_class,
                                   PROP_VIEW,
                                   g_param_spec_object ("view",
                                                        "View",
                                                        "The WebView associated to this widget",
                                                        EPHY_TYPE_WEB_VIEW,
                                                        G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB));

  g_type_class_add_private (klass, sizeof (EphyTitleDomainBoxPrivate));
}

static void
ephy_title_domain_box_init (EphyTitleDomainBox *box)
{
  EphyTitleDomainBoxPrivate *priv;

  priv = box->priv = EPHY_TITLE_DOMAIN_BOX_GET_PRIVATE (box);

  gtk_orientable_set_orientation (GTK_ORIENTABLE (box), GTK_ORIENTATION_VERTICAL);
  gtk_box_set_spacing (GTK_BOX (box), 0);

  priv->title = gtk_label_new ("");
  gtk_widget_set_halign (priv->title, GTK_ALIGN_CENTER);
  gtk_widget_show (priv->title);
  gtk_box_pack_start (GTK_BOX (box),
                      priv->title,
                      FALSE, FALSE, 0);

  priv->domain = gtk_label_new ("");
  gtk_widget_set_halign (priv->domain, GTK_ALIGN_CENTER);
  gtk_widget_show (priv->domain);
  gtk_box_pack_start (GTK_BOX (box),
                      priv->domain,
                      FALSE, FALSE, 0);
}

void
ephy_title_domain_box_set_view (EphyTitleDomainBox *box,
                                EphyWebView *view)
{
  EphyTitleDomainBoxPrivate *priv;

  g_return_if_fail (EPHY_IS_TITLE_DOMAIN_BOX (box));

  priv = box->priv;

  if (priv->view)
    g_object_unref (priv->view);

  priv->view = view;

  if (priv->view)
    g_object_ref (priv->view);

  reset_bindings (box);

  g_object_notify (G_OBJECT (box), "view");
}

GtkWidget *
ephy_title_domain_box_new (EphyWebView *view)
{
  return g_object_new (EPHY_TYPE_TITLE_DOMAIN_BOX,
                       "view", view,
                       NULL);
}

