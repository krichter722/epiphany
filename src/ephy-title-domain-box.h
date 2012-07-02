/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=2 sts=2 et: */
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

#if !defined (__EPHY_EPIPHANY_H_INSIDE__) && !defined (EPIPHANY_COMPILATION)
#error "Only <epiphany/epiphany.h> can be included directly."
#endif

#ifndef EPHY_TITLE_DOMAIN_BOX_H
#define EPHY_TITLE_DOMAIN_BOX_H

#include <gtk/gtk.h>

#include "ephy-web-view.h"

G_BEGIN_DECLS

#define EPHY_TYPE_TITLE_DOMAIN_BOX         (ephy_title_domain_box_get_type ())
#define EPHY_TITLE_DOMAIN_BOX(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), EPHY_TYPE_TITLE_DOMAIN_BOX, EphyTitleDomainBox))
#define EPHY_TITLE_DOMAIN_BOX_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), EPHY_TYPE_TITLE_DOMAIN_BOX, EphyTitleDomainBoxClass))
#define EPHY_IS_TITLE_DOMAIN_BOX(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), EPHY_TYPE_TITLE_DOMAIN_BOX))
#define EPHY_IS_TITLE_DOMAIN_BOX_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), EPHY_TYPE_TITLE_DOMAIN_BOX))
#define EPHY_TITLE_DOMAIN_BOX_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), EPHY_TYPE_TITLE_DOMAIN_BOX, EphyTitleDomainBoxClass))

typedef struct _EphyTitleDomainBoxClass    EphyTitleDomainBoxClass;
typedef struct _EphyTitleDomainBox         EphyTitleDomainBox;
typedef struct _EphyTitleDomainBoxPrivate  EphyTitleDomainBoxPrivate;

struct _EphyTitleDomainBox {
  GtkBox parent;

  /*< private >*/
  EphyTitleDomainBoxPrivate *priv;
};

struct _EphyTitleDomainBoxClass {
  GtkBoxClass parent_class;
};

GType       ephy_title_domain_box_get_type (void);
GtkWidget * ephy_title_domain_box_new      (EphyWebView *view);
void        ephy_title_domain_box_set_view (EphyTitleDomainBox *box,
                                            EphyWebView *view);

G_END_DECLS

#endif
