/*
 *  Copyright (C) 2003 Marco Pesenti Gritti
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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "ephy-favicon-action.h"
#include "eggtoolitem.h"

static void ephy_favicon_action_init       (EphyFaviconAction *action);
static void ephy_favicon_action_class_init (EphyFaviconActionClass *class);

static GObjectClass *parent_class = NULL;

GType
ephy_favicon_action_get_type (void)
{
	static GtkType type = 0;

	if (!type)
	{
		static const GTypeInfo type_info =
		{
			sizeof (EphyFaviconActionClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) ephy_favicon_action_class_init,
			(GClassFinalizeFunc) NULL,
			NULL,
			sizeof (EphyFaviconAction),
			0, /* n_preallocs */
			(GInstanceInitFunc) ephy_favicon_action_init,
		};

		type = g_type_register_static (EGG_TYPE_ACTION,
					       "EphyFaviconAction",
					       &type_info, 0);
	}
	return type;
}

static GtkWidget *
create_tool_item (EggAction *action)
{
	GtkWidget *image;
	GtkWidget *ebox;
	GtkWidget *item;

	item = (* EGG_ACTION_CLASS (parent_class)->create_tool_item) (action);

        ebox = gtk_event_box_new ();
	image = gtk_image_new ();
	gtk_container_add (GTK_CONTAINER (ebox), image);
	gtk_container_set_border_width (GTK_CONTAINER (ebox), 2);
	gtk_container_add (GTK_CONTAINER (item), ebox);
	gtk_widget_show (image);
	gtk_widget_show (ebox);

	return item;
}

static void
ephy_favicon_action_class_init (EphyFaviconActionClass *class)
{
	EggActionClass *action_class;

	parent_class = g_type_class_peek_parent (class);
	action_class = EGG_ACTION_CLASS (class);

	action_class->toolbar_item_type = EGG_TYPE_TOOL_ITEM;
	action_class->create_tool_item = create_tool_item;
}

static void
ephy_favicon_action_init (EphyFaviconAction *action)
{
}
