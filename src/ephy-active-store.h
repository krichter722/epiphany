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
 *  Author: Claudio Saavedra  <csaavedra@igalia.com>
 */

#ifndef _EPHY_ACTIVE_STORE_H
#define _EPHY_ACTIVE_STORE_H

#include <glib-object.h>

#include "ephy-notebook.h"
#include "ephy-overview-store.h"

G_BEGIN_DECLS

#define EPHY_TYPE_ACTIVE_STORE            (ephy_active_store_get_type())
#define EPHY_ACTIVE_STORE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), EPHY_TYPE_ACTIVE_STORE, EphyActiveStore))
#define EPHY_ACTIVE_STORE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), EPHY_TYPE_ACTIVE_STORE, EphyActiveStoreClass))
#define EPHY_IS_ACTIVE_STORE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EPHY_TYPE_ACTIVE_STORE))
#define EPHY_IS_ACTIVE_STORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), EPHY_TYPE_ACTIVE_STORE))
#define EPHY_ACTIVE_STORE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), EPHY_TYPE_ACTIVE_STORE, EphyActiveStoreClass))

typedef struct _EphyActiveStore        EphyActiveStore;
typedef struct _EphyActiveStoreClass   EphyActiveStoreClass;
typedef struct _EphyActiveStorePrivate EphyActiveStorePrivate;

struct _EphyActiveStore
{
  EphyOverviewStore parent;

  /*< priv >*/
  EphyActiveStorePrivate *priv;
};

struct _EphyActiveStoreClass
{
  EphyOverviewStoreClass parent_class;
};

enum {
  EPHY_ACTIVE_STORE_TAB_POS = EPHY_OVERVIEW_STORE_NCOLS,
  EPHY_ACTIVE_STORE_NCOLS
};

GType            ephy_active_store_get_type     (void) G_GNUC_CONST;

void             ephy_active_store_set_notebook (EphyActiveStore *store,
                                                 EphyNotebook *notebook);

EphyNotebook*    ephy_active_store_get_notebook (EphyActiveStore *store);

EphyActiveStore* ephy_active_store_new          (EphyNotebook *notebook);

G_END_DECLS

#endif /* _EPHY_ACTIVE_STORE_H */
