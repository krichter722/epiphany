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
 */

#ifndef MOZILLA_DOWNLOADER_VIEW_H
#define MOZILLA_DOWNLOADER_VIEW_H

#include "downloader-view.h"

#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

#define MOZILLA_TYPE_DOWNLOADER_VIEW		(mozilla_downloader_view_get_type ())
#define MOZILLA_DOWNLOADER_VIEW(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), MOZILLA_TYPE_DOWNLOADER_VIEW, MozillaDownloaderView))
#define MOZILLA_DOWNLOADER_VIEW_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), MOZILLA_TYPE_DOWNLOADER_VIEW, MozillaDownloaderViewClass))
#define MOZILLA_IS_DOWNLOADER_VIEW(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), MOZILLA_TYPE_DOWNLOADER_VIEW))
#define MOZILLA_IS_DOWNLOADER_VIEW_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), MOZILLA_TYPE_DOWNLOADER_VIEW))
#define MOZILLA_DOWNLOADER_VIEW_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), MOZILLA_TYPE_DOWNLOADER_VIEW, MozillaDownloaderViewClass))

typedef struct MozillaDownloaderViewClass MozillaDownloaderViewClass;
typedef struct MozillaDownloaderView MozillaDownloaderView;
typedef struct MozillaDownloaderViewPrivate MozillaDownloaderViewPrivate;

struct MozillaDownloaderView
{
        DownloaderView parent;
        MozillaDownloaderViewPrivate *priv;
};

struct MozillaDownloaderViewClass
{
        DownloaderViewClass parent_class;

	void (*cancel_download)    (DownloaderView *dv, gpointer persist_object);
	void (*pause_download)     (DownloaderView *dv, gpointer persist_object);
	void (*resume_download)    (DownloaderView *dv, gpointer persist_object);
};

GType               mozilla_downloader_view_get_type   (void);

void		    mozilla_downloader_view_completed  (MozillaDownloaderView *persist);

void		    mozilla_downloader_view_cancelled  (MozillaDownloaderView *persist);

G_END_DECLS

#endif
