/*
 *  Copyright (C) 2000-2003 Marco Pesenti Gritti
 *  Copyright (C) 2003 Christian Persch
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
 *
 */

#ifndef WEBCORE_EMBED_SINGLE_H
#define WEBCORE_EMBED_SINGLE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define WEBCORE_TYPE_EMBED_SINGLE		(webcore_embed_single_get_type ())
#define WEBCORE_EMBED_SINGLE(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), WEBCORE_TYPE_EMBED_SINGLE, WebcoreEmbedSingle))
#define WEBCORE_EMBED_SINGLE_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), WEBCORE_TYPE_EMBED_SINGLE, WebcoreEmbedSingleClass))
#define WEBCORE_IS_EMBED_SINGLE(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), WEBCORE_TYPE_EMBED_SINGLE))
#define WEBCORE_IS_EMBED_SINGLE_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), WEBCORE_TYPE_EMBED_SINGLE))
#define WEBCORE_EMBED_SINGLE_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), WEBCORE_TYPE_EMBED_SINGLE, WebcoreEmbedSingleClass))

typedef struct WebcoreEmbedSingle		WebcoreEmbedSingle;
typedef struct WebcoreEmbedSingleClass		WebcoreEmbedSingleClass;
typedef struct WebcoreEmbedSinglePrivate	WebcoreEmbedSinglePrivate;

struct WebcoreEmbedSingle
{
	GObject parent;

	/*< private >*/
	WebcoreEmbedSinglePrivate *priv;
};

struct WebcoreEmbedSingleClass
{
	GObjectClass parent_class;
};

GType	webcore_embed_single_get_type	(void);

G_END_DECLS

#endif
