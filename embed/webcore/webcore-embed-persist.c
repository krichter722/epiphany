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
#include "config.h"
#endif

#include "webcore-embed-persist.h"
#include "webcore-embed.h"
#include "ephy-embed-shell.h"
#include "ephy-file-helpers.h"
#include "EphyBrowser.h"
#include "EphyHeaderSniffer.h"
#include "MozDownload.h"
#include "EphyUtils.h"

#include <stddef.h>

static void
webcore_embed_persist_class_init (WebcoreEmbedPersistClass *klass);
static void
webcore_embed_persist_init (WebcoreEmbedPersist *ges);
static void
webcore_embed_persist_finalize (GObject *object);

#define WEBCORE_EMBED_PERSIST_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), WEBCORE_TYPE_EMBED_PERSIST, WebcoreEmbedPersistPrivate))

struct WebcoreEmbedPersistPrivate
{
	gpointer dummy;
};

static GObjectClass *parent_class = NULL;

GType
webcore_embed_persist_get_type (void)
{
       static GType webcore_embed_persist_type = 0;

        if (webcore_embed_persist_type == 0)
        {
                static const GTypeInfo our_info =
                {
                        sizeof (WebcoreEmbedPersistClass),
                        NULL, /* base_init */
                        NULL, /* base_finalize */
                        (GClassInitFunc) webcore_embed_persist_class_init,
                        NULL, /* class_finalize */
                        NULL, /* class_data */
                        sizeof (WebcoreEmbedPersist),
                        0,    /* n_preallocs */
                        (GInstanceInitFunc) webcore_embed_persist_init
                };

                webcore_embed_persist_type = 
				g_type_register_static (EPHY_TYPE_EMBED_PERSIST,
                                                        "WebcoreEmbedPersist",
                                                        &our_info, (GTypeFlags)0);
        }

        return webcore_embed_persist_type;
}

static void
webcore_embed_persist_init (WebcoreEmbedPersist *persist)
{
        persist->priv = WEBCORE_EMBED_PERSIST_GET_PRIVATE (persist);
}

static void
webcore_embed_persist_finalize (GObject *object)
{
        WebcoreEmbedPersist *persist = WEBCORE_EMBED_PERSIST (object);
        G_OBJECT_CLASS (parent_class)->finalize (object);
}

webcore_embed_persist_completed (WebcoreEmbedPersist *persist)
{
	g_signal_emit_by_name (persist, "completed");
	g_object_unref (persist);
}

void
webcore_embed_persist_cancelled (WebcoreEmbedPersist *persist)
{
	g_signal_emit_by_name (persist, "cancelled");
	g_object_unref (persist);
}

static void
impl_cancel (EphyEmbedPersist *persist)
{
	g_object_unref (persist);
}

static gboolean
impl_save (EphyEmbedPersist *persist)
{
	return FALSE;
}

static char *
impl_to_string (EphyEmbedPersist *persist)
{
	return NULL;
}

static void
webcore_embed_persist_class_init (WebcoreEmbedPersistClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
	EphyEmbedPersistClass *persist_class = EPHY_EMBED_PERSIST_CLASS (klass);
	
        parent_class = (GObjectClass *) g_type_class_peek_parent (klass);
	
        object_class->finalize = webcore_embed_persist_finalize;

	persist_class->save = impl_save;
	persist_class->cancel = impl_cancel;
	persist_class->to_string = impl_to_string;

	g_type_class_add_private (object_class, sizeof(WebcoreEmbedPersistPrivate));
}
