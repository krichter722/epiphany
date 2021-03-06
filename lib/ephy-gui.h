/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright © 2002 Marco Pesenti Gritti
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
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <gdk/gdkkeysyms.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

void            ephy_gui_sanitise_popup_position         (GtkMenu *menu,
                                                          GtkWidget *parent_widget,
                                                          gint *x,
                                                          gint *y);

void            ephy_gui_menu_position_tree_selection    (GtkMenu   *menu,
                                                          gint      *x,
                                                          gint      *y,
                                                          gboolean  *push_in,
                                                          gpointer  user_data);

void            ephy_gui_menu_position_under_widget      (GtkMenu   *menu,
                                                          gint      *x,
                                                          gint      *y,
                                                          gboolean  *push_in,
                                                          gpointer  user_data);

GtkWindowGroup *ephy_gui_ensure_window_group             (GtkWindow *window);

void            ephy_gui_get_current_event               (GdkEventType *type,
                                                          guint *state,
                                                          guint *button);

gboolean        ephy_gui_is_middle_click                 (void);

gboolean        ephy_gui_check_location_writable         (GtkWidget *parent,
                                                          const char *filename);

void            ephy_gui_help                            (GtkWidget *parent,
                                                          const char *page);

G_END_DECLS
