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

#ifndef EPHY_DOWNLOAD_H
#define EPHY_DOWNLOAD_H

#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

#define EPHY_TYPE_DOWNLOAD		(ephy_download_get_type ())
#define EPHY_DOWNLOAD(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), EPHY_TYPE_DOWNLOAD, EphyDownload))
#define EPHY_DOWNLOAD_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), EPHY_TYPE_DOWNLOAD, EphyDownloadClass))
#define EPHY_IS_DOWNLOAD(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), EPHY_TYPE_DOWNLOAD))
#define EPHY_IS_DOWNLOAD_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), EPHY_TYPE_DOWNLOAD))
#define EPHY_DOWNLOAD_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), EPHY_TYPE_DOWNLOAD, EphyDownloadClass))

typedef struct EphyDownload EphyDownload;
typedef struct EphyDownloadClass EphyDownloadClass;
typedef struct EphyDownloadPrivate EphyDownloadPrivate;

typedef enum
{
	EPHY_DOWNLOAD_STATUS_DOWNLOADING,
	EPHY_DOWNLOAD_STATUS_PAUSED,
	EPHY_DOWNLOAD_STATUS_COMPLETED
} EphyDownloadStatus;

struct EphyDownload
{
        GObject parent;
};

struct EphyDownloadClass
{
        GObjectClass parent_class;

	void (* cancel)             (EphyDownload *download);
	void (* pause)              (EphyDownload *download);
	void (* resume)             (EphyDownload *download);
};

GType           ephy_download_get_type          (void);

EphyDownload   *ephy_download_new               (void);

void		ephy_download_cancel	      	(EphyDownload *dv);

void		ephy_download_pause		(EphyDownload *dv);

void		ephy_download_resume		(EphyDownload *dv);

G_END_DECLS

#endif
