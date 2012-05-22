/*
 * Copyright (c) 2011 Red Hat, Inc.
 * Copyright (c) 2012 Igalia, S.L.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Based on gd-toggle-pixbuf-renderer by Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#include "config.h"

#include "ephy-removable-pixbuf-renderer.h"
#include "ephy-widgets-type-builtins.h"

G_DEFINE_TYPE (EphyRemovablePixbufRenderer, ephy_removable_pixbuf_renderer, GD_TYPE_TOGGLE_PIXBUF_RENDERER);

enum {
  DELETE_CLICKED,
  LAST_SIGNAL
};

enum {
  PROP_0 = 0,
  PROP_RENDER_POLICY
};

static guint signals[LAST_SIGNAL] = { 0 };

struct _EphyRemovablePixbufRendererPrivate {
  EphyRemovablePixbufRenderPolicy policy;
};

static void
get_icon_rectangle (GtkWidget *widget,
		    GtkCellRenderer *cell,
		    const GdkRectangle *cell_area,
		    GdkPixbuf *icon,
		    GdkRectangle *rectangle)
{
  GtkTextDirection direction;
  gint x_offset, xpad, ypad;
  gint icon_size;

  gtk_cell_renderer_get_padding (cell, &xpad, &ypad);
  direction = gtk_widget_get_direction (widget);
  icon_size = gdk_pixbuf_get_width (icon);

  if (direction == GTK_TEXT_DIR_RTL)
    x_offset = xpad;
  else
    x_offset = cell_area->width - icon_size - xpad;

  rectangle->x = cell_area->x + x_offset;
  rectangle->y = cell_area->y + ypad;
  rectangle->width = rectangle->height = icon_size;
}

static void
ephy_removable_pixbuf_renderer_render (GtkCellRenderer      *cell,
				       cairo_t              *cr,
				       GtkWidget            *widget,
				       const GdkRectangle   *background_area,
				       const GdkRectangle   *cell_area,
				       GtkCellRendererState  flags)
{
  GtkStyleContext *context;
  EphyRemovablePixbufRenderer *self = EPHY_REMOVABLE_PIXBUF_RENDERER (cell);
  GdkPixbuf *icon;
  GdkRectangle icon_area;

  GTK_CELL_RENDERER_CLASS (ephy_removable_pixbuf_renderer_parent_class)->render
    (cell, cr, widget,
     background_area, cell_area, flags);

  if (self->priv->policy ==  EPHY_REMOVABLE_PIXBUF_RENDER_NEVER ||
      (self->priv->policy == EPHY_REMOVABLE_PIXBUF_RENDER_PRELIT && !(flags & GTK_CELL_RENDERER_PRELIT)))
    return;

  /* gtk_widget_style_get (widget, */
  /*                       "check-icon-size", &icon_size, */
  /*                       NULL); */

  /* if (icon_size == -1) */
  /*   icon_size = 40; */

  icon = gtk_widget_render_icon_pixbuf (widget, GTK_STOCK_CLOSE,
					GTK_ICON_SIZE_BUTTON);
  get_icon_rectangle (widget, cell, cell_area, icon, &icon_area);
  context = gtk_widget_get_style_context (widget);
  gtk_render_icon (context, cr, icon, icon_area.x, icon_area.y);
  g_object_unref (icon);
}

static gboolean
ephy_removable_pixbuf_renderer_activate (GtkCellRenderer      *cell,
					 GdkEvent             *event,
					 GtkWidget            *widget,
					 const gchar          *path,
					 const GdkRectangle   *background_area,
					 const GdkRectangle   *cell_area,
					 GtkCellRendererState  flags)
{
  GdkEventButton *ev = (GdkEventButton *) event;
  GdkRectangle icon_area;
  GdkPixbuf *icon = gtk_widget_render_icon_pixbuf (widget, GTK_STOCK_CLOSE,
						   GTK_ICON_SIZE_BUTTON);
  get_icon_rectangle (widget, cell, cell_area, icon, &icon_area);
  if (icon_area.x <= ev->x && ev->x <= icon_area.x + icon_area.width &&
      icon_area.y <= ev->y && ev->y <= icon_area.y + icon_area.height) {
    g_signal_emit (cell, signals [DELETE_CLICKED], 0, path);
    return TRUE;
  }
  g_object_unref (icon);

  return FALSE;
}

static void
ephy_removable_pixbuf_renderer_get_property (GObject    *object,
					     guint       property_id,
					     GValue     *value,
					     GParamSpec *pspec)
{
  EphyRemovablePixbufRenderer *self = EPHY_REMOVABLE_PIXBUF_RENDERER (object);

  switch (property_id)
    {
    case PROP_RENDER_POLICY:
      g_value_set_enum (value, self->priv->policy);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
ephy_removable_pixbuf_renderer_set_property (GObject    *object,
					     guint       property_id,
					     const GValue *value,
					     GParamSpec *pspec)
{
  EphyRemovablePixbufRenderer *self = EPHY_REMOVABLE_PIXBUF_RENDERER (object);

  switch (property_id)
    {
    case PROP_RENDER_POLICY:
      self->priv->policy = g_value_get_enum (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
ephy_removable_pixbuf_renderer_class_init (EphyRemovablePixbufRendererClass *klass)
{
  GObjectClass *oclass = G_OBJECT_CLASS (klass);
  GtkCellRendererClass *crclass = GTK_CELL_RENDERER_CLASS (klass);

  crclass->render = ephy_removable_pixbuf_renderer_render;
  crclass->activate = ephy_removable_pixbuf_renderer_activate;
  oclass->get_property = ephy_removable_pixbuf_renderer_get_property;
  oclass->set_property = ephy_removable_pixbuf_renderer_set_property;

  g_object_class_install_property (oclass,
				   PROP_RENDER_POLICY,
				   g_param_spec_enum ("render-policy",
						      "Render policy",
						      "The rendering policy for the close icon in the renderer",
						      EPHY_TYPE_REMOVABLE_PIXBUF_RENDER_POLICY,
						      EPHY_REMOVABLE_PIXBUF_RENDER_PRELIT,
						      G_PARAM_CONSTRUCT |
						      G_PARAM_READWRITE |
						      G_PARAM_STATIC_STRINGS));

  signals[DELETE_CLICKED] =
    g_signal_new ("delete-clicked",
		  G_OBJECT_CLASS_TYPE (oclass),
		  G_SIGNAL_RUN_LAST,
		  0, NULL, NULL, NULL,
		  G_TYPE_NONE, 1,
		  G_TYPE_STRING);

  g_type_class_add_private (klass, sizeof (EphyRemovablePixbufRendererPrivate));
}

static void
ephy_removable_pixbuf_renderer_init (EphyRemovablePixbufRenderer *self)
{
  self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, EPHY_TYPE_REMOVABLE_PIXBUF_RENDERER,
                                            EphyRemovablePixbufRendererPrivate);
  g_object_set (self, "mode", GTK_CELL_RENDERER_MODE_ACTIVATABLE, NULL);
}

GtkCellRenderer *
ephy_removable_pixbuf_renderer_new (void)
{
  return g_object_new (EPHY_TYPE_REMOVABLE_PIXBUF_RENDERER, NULL);
}
