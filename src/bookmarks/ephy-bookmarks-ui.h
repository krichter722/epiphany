/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright © 2005 Peter Harvey
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
 */

#pragma once

#include "ephy-window.h"
#include "ephy-node.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define EPHY_BOOKMARKS_UI_ACTION_NAME_BUFFER_SIZE        32

#define EPHY_BOOKMARK_ACTION_NAME_BUFFER_SIZE            EPHY_BOOKMARKS_UI_ACTION_NAME_BUFFER_SIZE
#define EPHY_BOOKMARK_ACTION_NAME_FORMAT                 "Bmk%u"
#define EPHY_BOOKMARK_ACTION_NAME_FORMAT_ARG(node)       (ephy_node_get_id (node))
#define EPHY_BOOKMARK_ACTION_NAME_PRINTF(buffer,node)    (g_snprintf (buffer, sizeof (buffer), EPHY_BOOKMARK_ACTION_NAME_FORMAT, EPHY_BOOKMARK_ACTION_NAME_FORMAT_ARG (node)))
#define EPHY_BOOKMARK_ACTION_NAME_STRDUP_PRINTF(node)    (g_strdup_printf (EPHY_BOOKMARK_ACTION_NAME_FORMAT, EPHY_BOOKMARK_ACTION_NAME_FORMAT_ARG (node)))

#define EPHY_TOPIC_ACTION_NAME_BUFFER_SIZE               EPHY_BOOKMARKS_UI_ACTION_NAME_BUFFER_SIZE
#define EPHY_TOPIC_ACTION_NAME_FORMAT                    "Tp%u"
#define EPHY_TOPIC_ACTION_NAME_FORMAT_ARG(node)          (ephy_node_get_id (node))
#define EPHY_TOPIC_ACTION_NAME_PRINTF(buffer,node)       (g_snprintf (buffer, sizeof (buffer), EPHY_TOPIC_ACTION_NAME_FORMAT, EPHY_TOPIC_ACTION_NAME_FORMAT_ARG (node)))
#define EPHY_TOPIC_ACTION_NAME_STRDUP_PRINTF(node)       (g_strdup_printf (EPHY_TOPIC_ACTION_NAME_FORMAT, EPHY_TOPIC_ACTION_NAME_FORMAT_ARG (node)))

#define EPHY_OPEN_TABS_ACTION_NAME_BUFFER_SIZE           EPHY_BOOKMARKS_UI_ACTION_NAME_BUFFER_SIZE
#define EPHY_OPEN_TABS_ACTION_NAME_FORMAT                "OpTb%u"
#define EPHY_OPEN_TABS_ACTION_NAME_FORMAT_ARG(node)      (ephy_node_get_id (node))
#define EPHY_OPEN_TABS_ACTION_NAME_PRINTF(buffer,node)   (g_snprintf (buffer, sizeof (buffer), EPHY_OPEN_TABS_ACTION_NAME_FORMAT, EPHY_OPEN_TABS_ACTION_NAME_FORMAT_ARG (node)))
#define EPHY_OPEN_TABS_ACTION_NAME_STRDUP_PRINTF(node)   (g_strdup_printf (EPHY_OPEN_TABS_ACTION_NAME_FORMAT, EPHY_OPEN_TABS_ACTION_NAME_FORMAT_ARG (node)))

void        ephy_bookmarks_ui_attach_window                (EphyWindow *window);

void        ephy_bookmarks_ui_detach_window                (EphyWindow *window);

void        ephy_bookmarks_ui_add_bookmark                (GtkWindow *parent,
                                                           const char *location,
                                                           const char *title);

void        ephy_bookmarks_ui_show_bookmark                (GtkWindow *parent,
                                                            EphyNode  *bookmark);

G_END_DECLS
