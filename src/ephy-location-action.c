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

#include "ephy-location-action.h"
#include "ephy-location-entry.h"
#include "eggtoolitem.h"

static void ephy_location_action_init       (EphyLocationAction *action);
static void ephy_location_action_class_init (EphyLocationActionClass *class);

static GObjectClass *parent_class = NULL;

GType
ephy_location_action_get_type (void)
{
	static GtkType type = 0;

	if (!type)
	{
		static const GTypeInfo type_info =
		{
			sizeof (EphyLocationActionClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) ephy_location_action_class_init,
			(GClassFinalizeFunc) NULL,
			NULL,
			sizeof (EphyLocationAction),
			0, /* n_preallocs */
			(GInstanceInitFunc) ephy_location_action_init,
		};

		type = g_type_register_static (EGG_TYPE_ACTION,
					       "EphyLocationAction",
					       &type_info, 0);
	}
	return type;
}

static GtkWidget *
create_tool_item (EggAction *action)
{
	GtkWidget *item;
	GtkWidget *location;

	item = (* EGG_ACTION_CLASS (parent_class)->create_tool_item) (action);

	location = ephy_location_entry_new ();
	gtk_container_add (GTK_CONTAINER (item), location);
	egg_tool_item_set_expandable (EGG_TOOL_ITEM (item), TRUE);
	gtk_widget_show (location);

	return item;
}

static void
ephy_location_action_class_init (EphyLocationActionClass *class)
{
	EggActionClass *action_class;

	parent_class = g_type_class_peek_parent (class);
	action_class = EGG_ACTION_CLASS (class);

	action_class->toolbar_item_type = EGG_TYPE_TOOL_ITEM;
	action_class->create_tool_item = create_tool_item;
}

static void
ephy_location_action_init (EphyLocationAction *action)
{
}
