/*  vim:set ts=8 noet sw=8:
 *  Copyright (C) 2000-2004 Marco Pesenti Gritti
 *  Copyright (C) 2003, 2004 Christian Persch
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "webcore-embed-single.h"
#include "ephy-embed-single.h"
#include "ephy-cookie-manager.h"
#include "ephy-password-manager.h"
#include "ephy-permission-manager.h"

#include "glib.h"
#include <glib/gi18n.h>

#define WEBCORE_EMBED_SINGLE_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), WEBCORE_TYPE_EMBED_SINGLE, WebcoreEmbedSinglePrivate))

struct WebcoreEmbedSinglePrivate
{
	gpointer dummy;
};

static void webcore_embed_single_class_init	(WebcoreEmbedSingleClass *klass);
static void ephy_embed_single_iface_init	(EphyEmbedSingleIface *iface);
static void ephy_cookie_manager_iface_init	(EphyCookieManagerIface *iface);
static void ephy_password_manager_iface_init	(EphyPasswordManagerIface *iface);
static void ephy_permission_manager_iface_init	(EphyPermissionManagerIface *iface);
static void webcore_embed_single_init		(WebcoreEmbedSingle *ges);
static gboolean have_gnome_url_handler		(const gchar *protocol);

static GObjectClass *parent_class = NULL;

GType
webcore_embed_single_get_type (void)
{
       static GType type = 0;

        if (type == 0)
        {
                static const GTypeInfo our_info =
                {
                        sizeof (WebcoreEmbedSingleClass),
                        NULL, /* base_init */
                        NULL, /* base_finalize */
                        (GClassInitFunc) webcore_embed_single_class_init,
                        NULL, /* class_finalize */
                        NULL, /* class_data */
                        sizeof (WebcoreEmbedSingle),
                        0,    /* n_preallocs */
                        (GInstanceInitFunc) webcore_embed_single_init
                };

		static const GInterfaceInfo embed_single_info =
		{
			(GInterfaceInitFunc) ephy_embed_single_iface_init,
			NULL,
			NULL
		};

		static const GInterfaceInfo cookie_manager_info =
		{
			(GInterfaceInitFunc) ephy_cookie_manager_iface_init,
			NULL,
			NULL
		};

		static const GInterfaceInfo password_manager_info =
		{
			(GInterfaceInitFunc) ephy_password_manager_iface_init,
			NULL,
			NULL
		};

		static const GInterfaceInfo permission_manager_info =
		{
			(GInterfaceInitFunc) ephy_permission_manager_iface_init,
			NULL,
			NULL
		};

		type = g_type_register_static (G_TYPE_OBJECT,
					       "WebcoreEmbedSingle",
					       &our_info,
					       (GTypeFlags)0);

		g_type_add_interface_static (type,
					     EPHY_TYPE_EMBED_SINGLE,
					     &embed_single_info);

		g_type_add_interface_static (type,
					     EPHY_TYPE_COOKIE_MANAGER,
					     &cookie_manager_info);

		g_type_add_interface_static (type,
					     EPHY_TYPE_PASSWORD_MANAGER,
					     &password_manager_info);

		g_type_add_interface_static (type,
					     EPHY_TYPE_PERMISSION_MANAGER,
					     &permission_manager_info);
	}

        return type;
}

static void
webcore_embed_single_init (WebcoreEmbedSingle *mes)
{
 	mes->priv = WEBCORE_EMBED_SINGLE_GET_PRIVATE (mes);
}

/*static void
webcore_embed_single_dispose (GObject *object)
{
	WebcoreEmbedSingle *single = WEBCORE_EMBED_SINGLE (object);

	if (single->priv->mSingleObserver)
	{
		single->priv->mSingleObserver->Detach ();
		NS_RELEASE (single->priv->mSingleObserver);
		single->priv->mSingleObserver = nsnull;
	}
}*/

static void
webcore_embed_single_finalize (GObject *object)
{
}

static void
impl_clear_cache (EphyEmbedSingle *shell)
{
}

static void
impl_clear_auth_cache (EphyEmbedSingle *shell)
{
}

static void
impl_set_offline_mode (EphyEmbedSingle *shell,
		       gboolean offline)
{
}

static gboolean
impl_get_offline_mode (EphyEmbedSingle *shell)
{
	return FALSE;
}

static GList *
impl_get_printer_list (EphyEmbedSingle *shell)
{
	return NULL;
}

static GList *
impl_get_font_list (EphyEmbedSingle *shell,
		    const char *langGroup)
{
	return NULL;
}

static GList *
impl_list_cookies (EphyCookieManager *manager)
{
	return NULL;
}

static void
impl_remove_cookie (EphyCookieManager *manager,
		    const EphyCookie *cookie)
{
}

static void
impl_clear_cookies (EphyCookieManager *manager)
{
}
	
static GList *
impl_list_passwords (EphyPasswordManager *manager)
{
	return NULL;
}

static void
impl_remove_password (EphyPasswordManager *manager,
		      EphyPasswordInfo *info)
{
}

static void
impl_permission_manager_add (EphyPermissionManager *manager,
			     const char *host,
			     const char *type,
			     EphyPermission permission)
{
}

static void
impl_permission_manager_remove (EphyPermissionManager *manager,
				const char *host,
				const char *type)
{
}

static void
impl_permission_manager_clear (EphyPermissionManager *manager)
{
}

EphyPermission
impl_permission_manager_test (EphyPermissionManager *manager,
			      const char *host,
			      const char *type)
{
	return 0;
}

GList *
impl_permission_manager_list (EphyPermissionManager *manager,
			      const char *type)
{
	return NULL;
}

static GtkWidget * 
impl_open_window (EphyEmbedSingle *single,
		  EphyEmbed *parent,
		  const char *address,
		  const char *name,
		  const char *features)
{
	return NULL;
}

static void
webcore_embed_single_class_init (WebcoreEmbedSingleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	parent_class = (GObjectClass *) g_type_class_peek_parent (klass);

	/*object_class->dispose = webcore_embed_single_dispose;*/
	object_class->finalize = webcore_embed_single_finalize;

	g_type_class_add_private (object_class, sizeof (WebcoreEmbedSinglePrivate));
}

static void
ephy_embed_single_iface_init (EphyEmbedSingleIface *iface)
{
	iface->clear_cache = impl_clear_cache;
	iface->clear_auth_cache = impl_clear_auth_cache;
	iface->set_offline_mode = impl_set_offline_mode;
	iface->get_offline_mode = impl_get_offline_mode;
	iface->get_font_list = impl_get_font_list;
	iface->open_window = impl_open_window;
	iface->get_printer_list = impl_get_printer_list;
}

static void
ephy_cookie_manager_iface_init (EphyCookieManagerIface *iface)
{
	iface->list = impl_list_cookies;
	iface->remove = impl_remove_cookie;
	iface->clear = impl_clear_cookies;
}

static void
ephy_password_manager_iface_init (EphyPasswordManagerIface *iface)
{
	iface->add = NULL; /* not implemented yet */
	iface->remove = impl_remove_password;
	iface->list = impl_list_passwords;
}

static void
ephy_permission_manager_iface_init (EphyPermissionManagerIface *iface)
{
	iface->add = impl_permission_manager_add;
	iface->remove = impl_permission_manager_remove;
	iface->clear = impl_permission_manager_clear;
	iface->test = impl_permission_manager_test;
	iface->list = impl_permission_manager_list;
}
