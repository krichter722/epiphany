/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright (C) 2011 Red Hat Inc.
 *
 *  This file is part of Epiphany.
 *
 *  Epiphany is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Epiphany is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Epiphany.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors: Cosimo Cecchi <cosimoc@redhat.com>
 */

#pragma once

#include <gtk/gtk.h>

#define NAUTILUS_FLOATING_BAR_ACTION_ID_STOP 1

#define NAUTILUS_TYPE_FLOATING_BAR nautilus_floating_bar_get_type()
#define NAUTILUS_FLOATING_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), NAUTILUS_TYPE_FLOATING_BAR, NautilusFloatingBar))
#define NAUTILUS_FLOATING_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), NAUTILUS_TYPE_FLOATING_BAR, NautilusFloatingBarClass))
#define NAUTILUS_IS_FLOATING_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NAUTILUS_TYPE_FLOATING_BAR))
#define NAUTILUS_IS_FLOATING_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), NAUTILUS_TYPE_FLOATING_BAR))
#define NAUTILUS_FLOATING_BAR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), NAUTILUS_TYPE_FLOATING_BAR, NautilusFloatingBarClass))

typedef struct _NautilusFloatingBar NautilusFloatingBar;
typedef struct _NautilusFloatingBarClass NautilusFloatingBarClass;
typedef struct _NautilusFloatingBarDetails NautilusFloatingBarDetails;

struct _NautilusFloatingBar {
	GtkBox parent;
	NautilusFloatingBarDetails *priv;
};

struct _NautilusFloatingBarClass {
	GtkBoxClass parent_class;
};

/* GObject */
GType       nautilus_floating_bar_get_type  (void);

GtkWidget * nautilus_floating_bar_new              (const gchar *primary_label,
						    const gchar *details_label,
						    gboolean show_spinner);

void       nautilus_floating_bar_set_primary_label (NautilusFloatingBar *self,
						    const gchar *label);
void       nautilus_floating_bar_set_details_label (NautilusFloatingBar *self,
						    const gchar *label);
void        nautilus_floating_bar_set_labels        (NautilusFloatingBar *self,
						     const gchar *primary,
						     const gchar *detail);
void        nautilus_floating_bar_set_show_spinner (NautilusFloatingBar *self,
						    gboolean show_spinner);

void        nautilus_floating_bar_add_action       (NautilusFloatingBar *self,
						    const gchar *icon_name,
						    gint action_id);
void        nautilus_floating_bar_cleanup_actions  (NautilusFloatingBar *self);
