/*
 *  Copyright (C) 2000, 2001, 2002 Marco Pesenti Gritti
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ephy-download.h"

static void
ephy_download_class_init (EphyDownloadClass *klass);
static void
ephy_download_init (EphyDownload *dv);

static GObjectClass *parent_class = NULL;

GType
ephy_download_get_type (void)
{
       static GType ephy_download_type = 0;

        if (ephy_download_type == 0)
        {
                static const GTypeInfo our_info =
                {
                        sizeof (EphyDownloadClass),
                        NULL, /* base_init */
                        NULL, /* base_finalize */
                        (GClassInitFunc) ephy_download_class_init,
                        NULL, /* class_finalize */
                        NULL, /* class_data */
                        sizeof (EphyDownload),
                        0,    /* n_preallocs */
                        (GInstanceInitFunc) ephy_download_init
                };

                ephy_download_type = g_type_register_static (G_TYPE_OBJECT,
                                                             "EphyDownload",
                                                             &our_info, 0);
        }

        return ephy_download_type;
}

static void
ephy_download_class_init (EphyDownloadClass *klass)
{
        parent_class = g_type_class_peek_parent (klass);
}

static void
ephy_download_init (EphyDownload *dv)
{
}

EphyDownload *
ephy_download_new (void)
{
	return EPHY_DOWNLOAD (g_object_new (EPHY_TYPE_DOWNLOAD, NULL));
}

void
ephy_download_cancel (EphyDownload *download)
{
	EphyDownloadClass *klass = EPHY_DOWNLOAD_GET_CLASS (download);
	klass->cancel (download);
}

void
ephy_download_pause (EphyDownload *download)
{
	EphyDownloadClass *klass = EPHY_DOWNLOAD_GET_CLASS (download);
	klass->pause (download);
}

void
ephy_download_resume (EphyDownload *download)
{
	EphyDownloadClass *klass = EPHY_DOWNLOAD_GET_CLASS (download);
	klass->resume (download);
}
