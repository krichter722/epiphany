/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright © 2003, 2004 Marco Pesenti Gritti
 *  Copyright © 2003, 2004 Christian Persch
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

#include "ephy-link.h"
#include "ephy-link-action.h"
#include "ephy-node.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define EPHY_TYPE_BOOKMARK_ACTION                (ephy_bookmark_action_get_type ())
#define EPHY_BOOKMARK_ACTION(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), EPHY_TYPE_BOOKMARK_ACTION, EphyBookmarkAction))
#define EPHY_BOOKMARK_ACTION_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), EPHY_TYPE_BOOKMARK_ACTION, EphyBookmarkActionClass))
#define EPHY_IS_BOOKMARK_ACTION(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EPHY_TYPE_BOOKMARK_ACTION))
#define EPHY_IS_BOOKMARK_ACTION_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((obj), EPHY_TYPE_BOOKMARK_ACTION))
#define EPHY_BOOKMARK_ACTION_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), EPHY_TYPE_BOOKMARK_ACTION, EphyBookmarkActionClass))

typedef struct _EphyBookmarkAction               EphyBookmarkAction;
typedef struct _EphyBookmarkActionPrivate        EphyBookmarkActionPrivate;
typedef struct _EphyBookmarkActionClass          EphyBookmarkActionClass;

struct _EphyBookmarkAction
{
  EphyLinkAction parent_instance;

  /*< private >*/
  EphyBookmarkActionPrivate *priv;
};

struct _EphyBookmarkActionClass
{
  EphyLinkActionClass parent_class;
};


GType           ephy_bookmark_action_get_type           (void);

GtkAction      *ephy_bookmark_action_new                (EphyNode *node,
                                                         const char *name);

void            ephy_bookmark_action_set_bookmark       (EphyBookmarkAction *action,
                                                         EphyNode *node);

EphyNode       *ephy_bookmark_action_get_bookmark       (EphyBookmarkAction *action);

void            ephy_bookmark_action_updated            (EphyBookmarkAction *action);

void            ephy_bookmark_action_activate           (EphyBookmarkAction *action,
                                                         GtkWidget *widget,
                                                         EphyLinkFlags flags);

G_END_DECLS
