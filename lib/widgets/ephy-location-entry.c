/*
 *  Copyright (C) 2002  Ricardo Fernández Pascual
 *  Copyright (C) 2003  Marco Pesenti Gritti
 *  Copyright (C) 2003  Christian Persch
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
 *
 *  $Id$
 */

#include "ephy-tree-model-node.h"
#include "ephy-location-entry.h"
#include "ephy-marshal.h"
#include "ephy-debug.h"

#include <gtk/gtktoolbar.h>
#include <gtk/gtkliststore.h>
#include <gtk/gtkcomboboxentry.h>
#include <gtk/gtkentry.h>
#include <gtk/gtkwindow.h>
#include <string.h>

#define EPHY_LOCATION_ENTRY_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), EPHY_TYPE_LOCATION_ENTRY, EphyLocationEntryPrivate))

struct _EphyLocationEntryPrivate
{
	GtkListStore *combo_model;
	GtkWidget *combo;
	GtkWidget *entry;
	char *before_completion;
	gboolean user_changed;
	gboolean activation_mode;
};

static void ephy_location_entry_class_init (EphyLocationEntryClass *klass);
static void ephy_location_entry_init (EphyLocationEntry *le);
static void ephy_location_entry_finalize (GObject *o);

static GObjectClass *parent_class = NULL;

enum EphyLocationEntrySignalsEnum
{
	ACTIVATED,
	USER_CHANGED,
	LAST_SIGNAL
};
static gint EphyLocationEntrySignals[LAST_SIGNAL];

GType
ephy_location_entry_get_type (void)
{
	static GType ephy_location_entry_type = 0;

	if (ephy_location_entry_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (EphyLocationEntryClass),
			NULL,
			NULL,
			(GClassInitFunc) ephy_location_entry_class_init,
			NULL,
			NULL,
			sizeof (EphyLocationEntry),
			0,
			(GInstanceInitFunc) ephy_location_entry_init
		};

		ephy_location_entry_type = g_type_register_static (GTK_TYPE_TOOL_ITEM,
							           "EphyLocationEntry",
							           &our_info, 0);
	}

	return ephy_location_entry_type;
}

static gboolean
ephy_location_entry_set_tooltip (GtkToolItem *tool_item,
				 GtkTooltips *tooltips,
				 const char *tip_text,
				 const char *tip_private)
{
	EphyLocationEntry *entry = EPHY_LOCATION_ENTRY (tool_item);

	g_return_val_if_fail (EPHY_IS_LOCATION_ENTRY (entry), FALSE);

	gtk_tooltips_set_tip (tooltips, entry->priv->entry, tip_text, tip_private);

	return TRUE;
}

static void
ephy_location_entry_class_init (EphyLocationEntryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkToolItemClass *tool_item_class = GTK_TOOL_ITEM_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = ephy_location_entry_finalize;

	tool_item_class->set_tooltip = ephy_location_entry_set_tooltip;

	EphyLocationEntrySignals[ACTIVATED] = g_signal_new (
		"activated", G_OBJECT_CLASS_TYPE (klass),
		G_SIGNAL_RUN_FIRST | G_SIGNAL_RUN_LAST | G_SIGNAL_RUN_CLEANUP,
                G_STRUCT_OFFSET (EphyLocationEntryClass, activated),
		NULL, NULL,
		ephy_marshal_VOID__STRING_STRING,
		G_TYPE_NONE,
		2,
		G_TYPE_STRING,
		G_TYPE_STRING);
	EphyLocationEntrySignals[USER_CHANGED] = g_signal_new (
		"user_changed", G_OBJECT_CLASS_TYPE (klass),
		G_SIGNAL_RUN_FIRST | G_SIGNAL_RUN_LAST | G_SIGNAL_RUN_CLEANUP,
                G_STRUCT_OFFSET (EphyLocationEntryClass, user_changed),
		NULL, NULL,
		g_cclosure_marshal_VOID__VOID,
		G_TYPE_NONE,
		0,
		G_TYPE_NONE);

	g_type_class_add_private (object_class, sizeof (EphyLocationEntryPrivate));
}

static void
ephy_location_entry_activation_finished (EphyLocationEntry *entry)
{
	if (entry->priv->activation_mode)
	{
		GtkWidget *toolbar;

		entry->priv->activation_mode = FALSE;

		toolbar = gtk_widget_get_ancestor (GTK_WIDGET (entry),
						   GTK_TYPE_TOOLBAR);
		gtk_widget_hide (toolbar);
	}
}

static gboolean
location_focus_out_cb (GtkWidget *leidget, GdkEventFocus *event, EphyLocationEntry *le)
{
	ephy_location_entry_activation_finished (le);

	return FALSE;
}

static void
editable_changed_cb (GtkEditable *editable, EphyLocationEntry *e)
{
	EphyLocationEntryPrivate *p = e->priv;

	if (p->user_changed)
	{
		g_signal_emit (e, EphyLocationEntrySignals[USER_CHANGED], 0);
	}
}

static gboolean
entry_button_press_cb (GtkWidget *entry, GdkEventButton *event, EphyLocationEntry *le)
{
	if (event->button == 1 && event->type == GDK_2BUTTON_PRESS)
	{
		ephy_location_entry_activate (le);
		return TRUE;
	}

	return FALSE;
}

static void
entry_activate_cb (GtkEntry *entry, EphyLocationEntry *e)
{
	char *content;

	content = gtk_editable_get_chars (GTK_EDITABLE(entry), 0, -1);
	g_signal_emit (e, EphyLocationEntrySignals[ACTIVATED], 0,
		       NULL, content);
	g_free (content);
}

static void
ephy_location_entry_construct_contents (EphyLocationEntry *le)
{
	EphyLocationEntryPrivate *p = le->priv;

	LOG ("EphyLocationEntry constructing contents %p", le)

	p->combo_model = gtk_list_store_new (1, G_TYPE_STRING);
	p->combo = gtk_combo_box_entry_new (GTK_TREE_MODEL (p->combo_model), 0);
	gtk_container_add (GTK_CONTAINER (le), p->combo);
	gtk_widget_show (p->combo);
	p->entry = GTK_BIN (p->combo)->child;

	g_signal_connect (p->entry, "button_press_event",
			  G_CALLBACK (entry_button_press_cb), le);
	g_signal_connect (p->entry, "changed",
			  G_CALLBACK (editable_changed_cb), le);
	g_signal_connect (p->entry, "activate",
			  G_CALLBACK (entry_activate_cb), le);
}

static void
ephy_location_entry_init (EphyLocationEntry *le)
{
	EphyLocationEntryPrivate *p;

	LOG ("EphyLocationEntry initialising %p", le)

	p = EPHY_LOCATION_ENTRY_GET_PRIVATE (le);
	le->priv = p;

	p->user_changed = TRUE;
	p->activation_mode = FALSE;

	ephy_location_entry_construct_contents (le);

	gtk_tool_item_set_expand (GTK_TOOL_ITEM (le), TRUE);

	g_signal_connect (le->priv->entry,
	       		  "focus_out_event",
                          G_CALLBACK (location_focus_out_cb), le);
}

static void
ephy_location_entry_finalize (GObject *o)
{
	EphyLocationEntry *le;
	
	le = EPHY_LOCATION_ENTRY (o);

	LOG ("EphyLocationEntry finalized")

	G_OBJECT_CLASS (parent_class)->finalize (o);
}

GtkWidget *
ephy_location_entry_new (void)
{
	return GTK_WIDGET (g_object_new (EPHY_TYPE_LOCATION_ENTRY, NULL));
}

void
ephy_location_entry_add_completion (EphyLocationEntry *le,
				    EphyNode *root,
				    guint text_property,
				    guint action_property)
{
	EphyTreeModelNode *node_model;
	GtkEntryCompletion *completion;
	int action_col;

	node_model = ephy_tree_model_node_new (root, NULL);
	ephy_tree_model_node_add_prop_column
		(node_model, G_TYPE_STRING, text_property);
	action_col = ephy_tree_model_node_add_prop_column
		(node_model, G_TYPE_STRING, action_property);

	completion = gtk_entry_completion_new ();
	gtk_entry_completion_set_model (completion, GTK_TREE_MODEL (node_model));
	gtk_entry_completion_set_text_column (completion, action_col);

	gtk_entry_set_completion (GTK_ENTRY (le->priv->entry), completion);
}

void
ephy_location_entry_set_location (EphyLocationEntry *le,
				  const gchar *new_location)
{
	EphyLocationEntryPrivate *p = le->priv;
	int pos;

	g_return_if_fail (new_location != NULL);

	p->user_changed = FALSE;

	gtk_editable_delete_text (GTK_EDITABLE (p->entry), 0, -1);
	gtk_editable_insert_text (GTK_EDITABLE (p->entry), new_location, strlen(new_location),
				  &pos);

	p->user_changed = TRUE;
}

const char *
ephy_location_entry_get_location (EphyLocationEntry *le)
{
	return gtk_entry_get_text (GTK_ENTRY (le->priv->entry));
}

void
ephy_location_entry_activate (EphyLocationEntry *le)
{
	GtkWidget *toplevel, *toolbar;

	toolbar = gtk_widget_get_ancestor (GTK_WIDGET (le), GTK_TYPE_TOOLBAR);

	if (!GTK_WIDGET_VISIBLE (toolbar))
	{
		le->priv->activation_mode = TRUE;

		gtk_widget_show (toolbar);
	}

	toplevel = gtk_widget_get_toplevel (le->priv->entry);

	gtk_editable_select_region (GTK_EDITABLE(le->priv->entry),
				    0, -1);
        gtk_window_set_focus (GTK_WINDOW(toplevel),
                              le->priv->entry);
}

void
ephy_location_entry_clear_history (EphyLocationEntry *le)
{
}
