/*
 *  Copyright (C) 2003 Xan Lopez
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
 * $Id$
 */

#include "mozilla-download.h"

static void
mozilla_download_class_init (MozillaDownloadClass *klass);
static void
mozilla_download_init (MozillaDownload *ges);
static void
mozilla_download_finalize (GObject *object);

static GObjectClass *parent_class = NULL;

GType
mozilla_download_get_type (void)
{
       static GType mozilla_download_type = 0;

        if (mozilla_download_type == 0)
        {
                static const GTypeInfo our_info =
                {
                        sizeof (MozillaDownloadClass),
                        NULL, /* base_init */
                        NULL, /* base_finalize */
                        (GClassInitFunc) mozilla_download_class_init,
                        NULL, /* class_finalize */
                        NULL, /* class_data */
                        sizeof (MozillaDownload),
                        0,    /* n_preallocs */
                        (GInstanceInitFunc) mozilla_download_init
                };

                mozilla_download_type = 
				g_type_register_static (EPHY_TYPE_DOWNLOAD,
                                                        "MozillaDownload",
                                                        &our_info, (GTypeFlags)0);
        }

        return mozilla_download_type;
}

static void 
impl_cancel (EphyDownload *download)
{
	MOZILLA_DOWNLOAD (download)->moz_download->Cancel ();
}

static void
impl_pause (EphyDownload *download)
{
}

static void
impl_resume (EphyDownload *download)
{
}

static void
mozilla_download_class_init (MozillaDownloadClass *klass)
{
	EphyDownloadClass *download_class = EPHY_DOWNLOAD_CLASS (klass);
	
        parent_class = (GObjectClass *) g_type_class_peek_parent (klass);
	
	download_class->cancel = impl_cancel;
	download_class->pause = impl_pause;
	download_class->resume = impl_resume;
}

static void
mozilla_download_init (MozillaDownload *view)
{
}

EphyDownload *
mozilla_download_new (void)
{
	return EPHY_DOWNLOAD (g_object_new (MOZILLA_TYPE_DOWNLOAD, NULL));
}
