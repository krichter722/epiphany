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

#include "EphyDownload.h"
#include "EphyWrapper.h"
#include "EphyHeaderSniffer.h"
#include "mozilla-embed.h"
#include "mozilla-downloader-view.h"

#include <stddef.h>
#include <nsIWebBrowserPersist.h>
#include <nsString.h>
#include <nsCWebBrowserPersist.h>
#include <nsNetUtil.h>
#include <nsIHistoryEntry.h>
#include <nsISHEntry.h>

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

#define MOZILLA_EMBED_PERSIST_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), MOZILLA_TYPE_EMBED_PERSIST, MozillaDownloaderViewPrivate))

struct MozillaDownloaderViewPrivate
{
	nsCOMPtr<nsIWebBrowserPersist> mPersist;
//	GProgressListener *mProgress;
};

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
mozilla_downloader_view_class_init (MozillaDownloaderViewClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
	DownloaderViewClass *view_class = EPHY_DOWNLOADER_VIEW_CLASS (klass);
	
        parent_class = (GObjectClass *) g_type_class_peek_parent (klass);
	
        object_class->finalize = mozilla_downloader_view_finalize;

	view_class->cancel_download = impl_cancel;
	view_class->pause_download = impl_pause;
	view_class->resume_download = impl_resume;

	g_type_class_add_private (object_class, sizeof(MozillaDownloaderViewPrivate));
}

static void
mozilla_downloader_view_init (MozillaDownloaderView *view)
{
//        view->priv = MOZILLA_DOWNLOADER_VIEW_GET_PRIVATE (view);

//     	view->priv->mPersist = do_CreateInstance (NS_WEBBROWSERPERSIST_CONTRACTID);
}

static void
mozilla_downloader_view_finalize (GObject *object)
{
        MozillaDownloaderView *view = MOZILLA_DOWNLOADER_VIEW (object);

//	view->priv->mPersist->SetProgressListener (nsnull);
//	view->priv->mPersist = nsnull;

        G_OBJECT_CLASS (parent_class)->finalize (object);
}

void
mozilla_downloader_view_completed (MozillaDownloaderView *view)
{
	g_signal_emit_by_name (view, "completed");
	g_object_unref (view);
}

void
mozilla_downloader_view_cancelled (MozillaDownloaderView *view)
{
	g_object_unref (view);
}

static void 
impl_cancel (DownloaderView *view, gpointer persist_object)
{
/*	nsCOMPtr<nsIWebBrowserPersist> bview =
	MOZILLA_EMBED_PERSIST (view)->priv->mPersist;
	if (!bview) return G_FAILED;

	bview->CancelSave ();

	g_object_unref (view);

	return G_OK;*/
}

static void
impl_pause (DownloaderView *view, gpointer persist_object)
{
}

static void
impl_resume (DownloaderView *view, gpointer persist_object)
{
}
