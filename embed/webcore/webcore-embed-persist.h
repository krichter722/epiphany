/*
 *  Copyright (C) 2000-2003 Marco Pesenti Gritti
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

#ifndef WEBCORE_EMBED_PERSIST_H
#define WEBCORE_EMBED_PERSIST_H

#include "ephy-embed-persist.h"

#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

#define WEBCORE_TYPE_EMBED_PERSIST		(webcore_embed_persist_get_type ())
#define WEBCORE_EMBED_PERSIST(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), WEBCORE_TYPE_EMBED_PERSIST, WebcoreEmbedPersist))
#define WEBCORE_EMBED_PERSIST_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), WEBCORE_TYPE_EMBED_PERSIST, WebcoreEmbedPersistClass))
#define WEBCORE_IS_EMBED_PERSIST(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), WEBCORE_TYPE_EMBED_PERSIST))
#define WEBCORE_IS_EMBED_PERSIST_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), WEBCORE_TYPE_EMBED_PERSIST))
#define WEBCORE_EMBED_PERSIST_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), WEBCORE_TYPE_EMBED_PERSIST, WebcoreEmbedPersistClass))

typedef struct WebcoreEmbedPersistClass WebcoreEmbedPersistClass;
typedef struct WebcoreEmbedPersist WebcoreEmbedPersist;
typedef struct WebcoreEmbedPersistPrivate WebcoreEmbedPersistPrivate;

struct WebcoreEmbedPersist
{
	EphyEmbedPersist parent;

	/*< private >*/
	WebcoreEmbedPersistPrivate *priv;
};

struct WebcoreEmbedPersistClass
{
	EphyEmbedPersistClass parent_class;
};

GType	webcore_embed_persist_get_type	(void);

void	webcore_embed_persist_completed	(WebcoreEmbedPersist *persist);

void	webcore_embed_persist_cancelled	(WebcoreEmbedPersist *persist);

G_END_DECLS

#endif
