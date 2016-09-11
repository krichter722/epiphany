/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright © 2003 Xan Lopez <xan@masilla.org>
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

G_BEGIN_DECLS

/* Databases */
#define EPHY_NODE_DB_HISTORY   "EphyHistory"
#define EPHY_NODE_DB_BOOKMARKS "EphyBookmarks"
#define EPHY_NODE_DB_SITEICONS "EphySiteIcons"
#define EPHY_NODE_DB_STATES    "EphyStates"

/* Root nodes */
enum
{
  BOOKMARKS_NODE_ID = 0,
  KEYWORDS_NODE_ID = 1,
  FAVORITES_NODE_ID = 2,
  BMKS_NOTCATEGORIZED_NODE_ID = 3,
  STATES_NODE_ID = 4,
  HOSTS_NODE_ID = 5,
  PAGES_NODE_ID = 6,
  ICONS_NODE_ID = 9,
  SMARTBOOKMARKS_NODE_ID = 10,
  BMKS_LOCAL_NODE_ID = 11,
};

typedef enum
{
  EPHY_NODE_ALL_PRIORITY,
  EPHY_NODE_SPECIAL_PRIORITY,
  EPHY_NODE_NORMAL_PRIORITY
} EphyNodePriority;

G_END_DECLS
