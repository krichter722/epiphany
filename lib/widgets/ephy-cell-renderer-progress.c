/* gtkcellrenderer.c
 * Copyright (C) 2002 Naba Kumar <kh_naba@users.sourceforge.net>
 * heavily modified by Jörgen Scheibengruber <mfcn@gmx.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include <stdlib.h>
#include "ephy-cell-renderer-progress.h"

static void ephy_cell_renderer_progress_init       (EphyCellRendererProgress *celltext);
static void ephy_cell_renderer_progress_class_init (EphyCellRendererProgressClass *class);

#define EPHY_CELL_RENDERER_PROGRESS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), EPHY_TYPE_CELL_RENDERER_PROGRESS, EphyCellRendererProgressPrivate))

enum
{
	PROP_0,
	PROP_VALUE
}; 

struct _EphyCellRendererProgressPrivate
{
	int value;
};

static gpointer parent_class;

GtkType
ephy_cell_renderer_progress_get_type (void)
{
	static GtkType cell_progress_type = 0;

	if (!cell_progress_type)
	{
		static const GTypeInfo cell_progress_info =
		{
			sizeof (EphyCellRendererProgressClass),
			NULL,		/* base_init */
			NULL,		/* base_finalize */
			(GClassInitFunc) ephy_cell_renderer_progress_class_init,
			NULL,		/* class_finalize */
			NULL,		/* class_data */
			sizeof (EphyCellRendererProgress),
			0,              /* n_preallocs */
			(GInstanceInitFunc) ephy_cell_renderer_progress_init,
		};
		cell_progress_type = g_type_register_static (GTK_TYPE_CELL_RENDERER,
                                               "EphyCellRendererProgress",
                                               &cell_progress_info, 0);
	}

	return cell_progress_type;
}

static void
ephy_cell_renderer_progress_init (EphyCellRendererProgress *cellprogress)
{
	cellprogress->priv = EPHY_CELL_RENDERER_PROGRESS_GET_PRIVATE (cellprogress);
	cellprogress->priv->value = 0;
}

static void
ephy_cell_renderer_progress_get_property (GObject *object,
                                          guint param_id,
                                          GValue *value,
                                          GParamSpec *pspec)
{
	EphyCellRendererProgress *cellprogress = EPHY_CELL_RENDERER_PROGRESS (object);

	switch (param_id)
	{
		case PROP_VALUE:
			g_value_set_int (value, cellprogress->priv->value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
	}
}

static void
ephy_cell_renderer_progress_set_property (GObject *object,
				          guint param_id,
				          const GValue *value,
				          GParamSpec   *pspec)
{
	EphyCellRendererProgress *cellprogress = EPHY_CELL_RENDERER_PROGRESS (object);

	switch (param_id)
	{
		case PROP_VALUE:
			cellprogress->priv->value = g_value_get_int (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
	}
	g_object_notify (object, "value");
}

static void
ephy_cell_renderer_progress_get_size (GtkCellRenderer *cell,
				      GtkWidget *widget,
				      GdkRectangle *cell_area,
				      gint *x_offset,
				      gint *y_offset,
				      gint *width,
				      gint *height)
{
	/* Always return 1 here. Doesn't make to much sense,
	 * but providing the real width would make it
	 * impossible for the bar to shrink again.
	 */

	if (width)
	{
		*width = 1;
	}

	if (height)
	{
		*height = 26;
	}
}

GtkCellRenderer*
ephy_cell_renderer_progress_new (void)
{
	return GTK_CELL_RENDERER (g_object_new (ephy_cell_renderer_progress_get_type (), NULL));
}

static void
ephy_cell_renderer_progress_render (GtkCellRenderer *cell,
			            GdkWindow *window,
			            GtkWidget *widget,
			            GdkRectangle *background_area,
			            GdkRectangle *cell_area,
			            GdkRectangle *expose_area,
			            guint flags)
{
	EphyCellRendererProgress *cellprogress = (EphyCellRendererProgress *) cell;
	GtkStateType state;
	GdkGC *gc;
	PangoLayout *layout;
	PangoRectangle logical_rect;
	char *text; 
	int x, y, w, h, perc_w, pos;
	
#define MAXWIDTH 150
#define MINWIDTH 60

	gc = gdk_gc_new (window);

	x = cell_area->x + 4;
	y = cell_area->y + 2;
	if (cell_area->width > MINWIDTH)
	{
		if (cell_area->width < MAXWIDTH)
		{
			w = cell_area->width - 8;
		}
		else
		{
			w = MAXWIDTH - 8;
		}
	}
	else
	{
		w = MINWIDTH - 8;
	}

	h = cell_area->height - 4;
	
	gdk_gc_set_rgb_fg_color (gc, &widget->style->fg[GTK_STATE_NORMAL]);
	gdk_draw_rectangle (window, gc, TRUE, x, y, w, h);

	gdk_gc_set_rgb_fg_color (gc, &widget->style->bg[GTK_STATE_NORMAL]);
	gdk_draw_rectangle (window, gc, TRUE, x + 1, y + 1, w - 2, h - 2);
	gdk_gc_set_rgb_fg_color (gc, &widget->style->bg[GTK_STATE_SELECTED]);
	perc_w = w - 4;
	perc_w = (int) (perc_w * cellprogress->priv->value / 100);
	gdk_draw_rectangle (window, gc, TRUE, x + 2, y + 2, perc_w, h - 4);
	
	text = g_strdup_printf ("%d %%", cellprogress->priv->value);
	layout = gtk_widget_create_pango_layout (widget, text);
	pango_layout_get_pixel_extents (layout, NULL, &logical_rect);
	g_object_unref (G_OBJECT (layout));
	g_free (text);
	text = g_strdup_printf ("%d %%", cellprogress->priv->value);
	layout = gtk_widget_create_pango_layout (widget, text);
	g_free (text);
	
	pos = (w - logical_rect.width)/2;
	
	if (perc_w < pos + logical_rect.width/2)
	{
		state = GTK_STATE_NORMAL;
	}
	else
	{
		state = GTK_STATE_SELECTED;
	}

	gtk_paint_layout (widget->style, window, state,
			  FALSE, cell_area, widget, "progressbar",
                          x + pos, y + (h - logical_rect.height)/2,
                          layout);

	g_object_unref (G_OBJECT (layout));
	g_object_unref (G_OBJECT (gc));
}

static void
ephy_cell_renderer_progress_class_init (EphyCellRendererProgressClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	GtkCellRendererClass *cell_class = GTK_CELL_RENDERER_CLASS (class);

	parent_class = g_type_class_peek_parent (class);
  
	object_class->get_property = ephy_cell_renderer_progress_get_property;
	object_class->set_property = ephy_cell_renderer_progress_set_property;

	cell_class->get_size = ephy_cell_renderer_progress_get_size;
	cell_class->render = ephy_cell_renderer_progress_render;
  
	g_object_class_install_property (object_class,
                                         PROP_VALUE,
                                         g_param_spec_int ("value",
                                                           "Value",
                                                           "Value of the progress bar.",
                                                           0, 100, 0,
                                                           G_PARAM_READWRITE));

	g_type_class_add_private (object_class, sizeof (EphyCellRendererProgressPrivate));
}
