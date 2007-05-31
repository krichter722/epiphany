/*
 *  Copyright © 2000-2004 Marco Pesenti Gritti
 *  Copyright © 2007 Igalia
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
 *  $Id: webkit-embed.h 7025 2007-05-10 17:39:37Z xrcalvar $
 */

#ifndef WEBKIT_EMBED_H
#define WEBKIT_EMBED_H

#include <glib.h>
#include <glib-object.h>

#include <gtkmozembed.h>

#include "ephy-embed.h"

G_BEGIN_DECLS

#define WEBKIT_TYPE_EMBED		(webkit_embed_get_type ())
#define WEBKIT_EMBED(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), WEBKIT_TYPE_EMBED, WebkitEmbed))
#define WEBKIT_EMBED_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), WEBKIT_TYPE_EMBED, WebkitEmbedClass))
#define WEBKIT_IS_EMBED(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), WEBKIT_TYPE_EMBED))
#define WEBKIT_IS_EMBED_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), WEBKIT_TYPE_EMBED))
#define WEBKIT_EMBED_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), WEBKIT_TYPE_EMBED, WebkitEmbedClass))

typedef struct WebkitEmbedClass	WebkitEmbedClass;
typedef struct WebkitEmbed		WebkitEmbed;
typedef struct WebkitEmbedPrivate	WebkitEmbedPrivate;

struct WebkitEmbed
{
        GtkMozEmbed parent;

	/*< private >*/
        WebkitEmbedPrivate *priv;
};

struct WebkitEmbedClass
{
        GtkMozEmbedClass parent_class;
};

GType	         webkit_embed_get_type         (void);

/* The following are private to the embed implementation */

gpointer        _webkit_embed_get_ephy_browser (WebkitEmbed *embed);

GtkMozEmbed    *_webkit_embed_new_xul_dialog   (void);

EphyEmbedChrome _webkit_embed_translate_chrome	(GtkMozEmbedChromeFlags flags);

G_END_DECLS

#endif
