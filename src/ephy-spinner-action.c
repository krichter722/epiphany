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

#include "ephy-spinner-action.h"
#include "ephy-spinner.h"
#include "eggtoolitem.h"

static void ephy_spinner_action_init       (EphySpinnerAction *action);
static void ephy_spinner_action_class_init (EphySpinnerActionClass *class);

static GObjectClass *parent_class = NULL;

GType
ephy_spinner_action_get_type (void)
{
	static GtkType type = 0;

	if (!type)
	{
		static const GTypeInfo type_info =
		{
			sizeof (EphySpinnerActionClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) ephy_spinner_action_class_init,
			(GClassFinalizeFunc) NULL,
			NULL,
			sizeof (EphySpinnerAction),
			0, /* n_preallocs */
			(GInstanceInitFunc) ephy_spinner_action_init,
		};

		type = g_type_register_static (EGG_TYPE_ACTION,
					       "EphySpinnerAction",
					       &type_info, 0);
	}
	return type;
}

static GtkWidget *
create_tool_item (EggAction *action)
{
	GtkWidget *item;
	GtkWidget *spinner;

	item = (* EGG_ACTION_CLASS (parent_class)->create_tool_item) (action);

	spinner = ephy_spinner_new ();
	ephy_spinner_set_small_mode (EPHY_SPINNER (spinner), TRUE);
	gtk_container_add (GTK_CONTAINER (item), spinner);
	egg_tool_item_set_pack_end (EGG_TOOL_ITEM (item), TRUE);
	gtk_widget_show (spinner);

	return item;
}

static void
ephy_spinner_action_class_init (EphySpinnerActionClass *class)
{
	EggActionClass *action_class;

	parent_class = g_type_class_peek_parent (class);
	action_class = EGG_ACTION_CLASS (class);

	action_class->toolbar_item_type = EGG_TYPE_TOOL_ITEM;
	action_class->create_tool_item = create_tool_item;
}

static void
ephy_spinner_action_init (EphySpinnerAction *action)
{
}
