/*
 *  Copyright (C) 2000-2004 Marco Pesenti Gritti
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

#include "ephy-embed-factory.h"

#ifdef ENABLE_MOZILLA_EMBED
#include "mozilla-embed.h"
#include "mozilla-embed-persist.h"
#include "mozilla-embed-single.h"
#elif ENABLE_WEBCORE_EMBED
#include "webcore-embed.h"
#include "webcore-embed-persist.h"
#include "webcore-embed-single.h"
#endif

#include "ephy-embed.h"
#include "ephy-embed-persist.h"
#include "ephy-embed-single.h"

/**
 * ephy_embed_factory_new_object:
 * @type: a #GType specifying which object to create
 * 
 * Create an instance of an object implementing the @type interface.
 *
 * Return value: the object instance
 **/
GObject	*
ephy_embed_factory_new_object (GType type)
{
	GObject *object = NULL;

	if (type == EPHY_TYPE_EMBED)
	{
#ifdef ENABLE_MOZILLA_EMBED
		object = g_object_new (MOZILLA_TYPE_EMBED, NULL);
#elif ENABLE_WEBCORE_EMBED
		object = g_object_new (WEBCORE_TYPE_EMBED, NULL);
#endif
	}
	else if (type == EPHY_TYPE_EMBED_PERSIST)
	{
#ifdef ENABLE_MOZILLA_EMBED
		object = g_object_new (MOZILLA_TYPE_EMBED_PERSIST, NULL);
#elif ENABLE_WEBCORE_EMBED
		object = g_object_new (WEBCORE_TYPE_EMBED_PERSIST, NULL);
#endif
	}
	else if (type == EPHY_TYPE_EMBED_SINGLE)
	{
#ifdef ENABLE_MOZILLA_EMBED
		object = g_object_new (MOZILLA_TYPE_EMBED_SINGLE, NULL);
#elif ENABLE_WEBCORE_EMBED
		object = g_object_new (WEBCORE_TYPE_EMBED_SINGLE, NULL);
#endif
	}
	else
	{
		g_assert_not_reached ();
	}

	return object;
}
