/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright © 2002 Marco Pesenti Gritti <mpeseng@tin.it>
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

#include "ephy-bookmarks.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define EPHY_TYPE_TOPICS_PALETTE (ephy_topics_palette_get_type ())
G_DECLARE_FINAL_TYPE (EphyTopicsPalette, ephy_topics_palette, EPHY, TOPICS_PALETTE, GtkListStore);

enum
{
  EPHY_TOPICS_PALETTE_COLUMN_TITLE,
  EPHY_TOPICS_PALETTE_COLUMN_NODE,
  EPHY_TOPICS_PALETTE_COLUMN_SELECTED,
  EPHY_TOPICS_PALETTE_COLUMNS
};

EphyTopicsPalette *ephy_topics_palette_new         (EphyBookmarks     *bookmarks,
                                                    EphyNode          *bookmark);

void               ephy_topics_palette_update_list (EphyTopicsPalette *self);

G_END_DECLS
