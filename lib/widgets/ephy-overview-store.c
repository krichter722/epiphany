/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright © 2012 Igalia S.L.
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "ephy-overview-store.h"

G_DEFINE_TYPE (EphyOverviewStore, ephy_overview_store, GTK_TYPE_LIST_STORE)

static void
ephy_overview_store_class_init (EphyOverviewStoreClass *klass)
{
}

static void
ephy_overview_store_init (EphyOverviewStore *self)
{
  GType types[EPHY_OVERVIEW_STORE_NCOLS];

  types[EPHY_OVERVIEW_STORE_ID] = G_TYPE_STRING;
  types[EPHY_OVERVIEW_STORE_URI] = G_TYPE_STRING;
  types[EPHY_OVERVIEW_STORE_TITLE] = G_TYPE_STRING;
  types[EPHY_OVERVIEW_STORE_AUTHOR] = G_TYPE_STRING;
  types[EPHY_OVERVIEW_STORE_SNAPSHOT] = GDK_TYPE_PIXBUF;
  types[EPHY_OVERVIEW_STORE_LAST_VISIT] = G_TYPE_LONG;
  types[EPHY_OVERVIEW_STORE_SELECTED] = G_TYPE_BOOLEAN;

  gtk_list_store_set_column_types (GTK_LIST_STORE (self),
                                   EPHY_OVERVIEW_STORE_NCOLS, types);
}
