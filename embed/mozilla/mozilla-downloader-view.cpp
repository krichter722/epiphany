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
 *  $Id$
 */

#include "EphyDownload.h"
#include "mozilla-downloader-view.h"

static void
mozilla_downloader_view_class_init (MozillaDownloaderViewClass *klass);
static void
mozilla_downloader_view_init (MozillaDownloaderView *ges);
static void
mozilla_downloader_view_finalize (GObject *object);

static void 
impl_cancel (DownloaderView *view, gpointer persist_object);

static void 
impl_pause (DownloaderView *view, gpointer persist_object);

static void 
impl_resume (DownloaderView *view, gpointer);

static GObjectClass *parent_class = NULL;

GType
mozilla_downloader_view_get_type (void)
{
       static GType mozilla_downloader_view_type = 0;

        if (mozilla_downloader_view_type == 0)
        {
                static const GTypeInfo our_info =
                {
                        sizeof (MozillaDownloaderViewClass),
                        NULL, /* base_init */
                        NULL, /* base_finalize */
                        (GClassInitFunc) mozilla_downloader_view_class_init,
                        NULL, /* class_finalize */
                        NULL, /* class_data */
                        sizeof (MozillaDownloaderView),
                        0,    /* n_preallocs */
                        (GInstanceInitFunc) mozilla_downloader_view_init
                };

                mozilla_downloader_view_type = 
				g_type_register_static (EPHY_TYPE_DOWNLOADER_VIEW,
                                                        "MozillaDownloaderView",
                                                        &our_info, (GTypeFlags)0);
        }

        return mozilla_downloader_view_type;
}

static void 
impl_cancel (DownloaderView *view, gpointer persist_object)
{
	EphyDownload *download = (EphyDownload*)persist_object;
	download->Cancel();
}

static void
impl_pause (DownloaderView *view, gpointer persist_object)
{
}

static void
impl_resume (DownloaderView *view, gpointer persist_object)
{
}
static void
mozilla_downloader_view_class_init (MozillaDownloaderViewClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
	DownloaderViewClass *view_class = EPHY_DOWNLOADER_VIEW_CLASS (klass);
	
        parent_class = (GObjectClass *) g_type_class_peek_parent (klass);
	
        object_class->finalize = mozilla_downloader_view_finalize;

	view_class->cancel_download = impl_cancel;
	view_class->pause_download = impl_pause;
	view_class->resume_download = impl_resume;
}

static void
mozilla_downloader_view_init (MozillaDownloaderView *view)
{
}

static void
mozilla_downloader_view_finalize (GObject *object)
{
        MozillaDownloaderView *view = MOZILLA_DOWNLOADER_VIEW (object);

        G_OBJECT_CLASS (parent_class)->finalize (object);
}

MozillaDownloaderView*
mozilla_downloader_view_new (void)
{
	return MOZILLA_DOWNLOADER_VIEW (g_object_new (MOZILLA_TYPE_DOWNLOADER_VIEW, NULL));
}
