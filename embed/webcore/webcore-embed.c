/*
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
 *  $Id$
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gtk-khtml.h>
#include "ephy-command-manager.h"
#include "webcore-embed.h"

static void	webcore_embed_class_init	(WebcoreEmbedClass *klass);
static void	webcore_embed_init		(WebcoreEmbed *gs);
static void	webcore_embed_destroy		(GtkObject *object);
static void	webcore_embed_finalize		(GObject *object);
static void	ephy_embed_iface_init		(EphyEmbedIface *iface);

static void webcore_embed_location_changed_cb	(GtkKHTML *embed,
						 WebcoreEmbed *membed);
static void webcore_embed_net_state_all_cb	(GtkKHTML *embed,
						 const char *aURI,
						 gint state,
						 guint status,
						 WebcoreEmbed *membed);
static gint webcore_embed_dom_key_down_cb	(GtkKHTML *embed,
						 gpointer dom_event,
						 WebcoreEmbed *membed);
static gint webcore_embed_dom_mouse_click_cb	(GtkKHTML *embed,
						 gpointer dom_event,
						 WebcoreEmbed *membed);
static gint webcore_embed_dom_mouse_down_cb	(GtkKHTML *embed,
						 gpointer dom_event, 
						 WebcoreEmbed *membed);
static void webcore_embed_new_window_cb		(GtkKHTML *embed, 
						 GtkKHTML **newEmbed,
						 guint chromemask,
						 WebcoreEmbed *membed);
static void webcore_embed_security_change_cb	(GtkKHTML *embed, 
						 gpointer request,
						 guint state, WebcoreEmbed *membed);
static EmbedSecurityLevel webcore_embed_security_level (WebcoreEmbed *membed);

#define WEBCORE_EMBED_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), WEBCORE_TYPE_EMBED, WebcoreEmbedPrivate))

#undef GTK_KHTML
#define GTK_KHTML(obj) (G_TYPE_CHECK_INSTANCE_CAST((GTK_BIN(obj)->child), gtk_khtml_get_type(), GtkKHTML))

typedef enum
{
	WEBCORE_EMBED_LOAD_STARTED,
	WEBCORE_EMBED_LOAD_REDIRECTING,
	WEBCORE_EMBED_LOAD_LOADING,
	WEBCORE_EMBED_LOAD_STOPPED
} WebcoreEmbedLoadState;

struct WebcoreEmbedPrivate
{
	guint security_state;
	WebcoreEmbedLoadState load_state;
};

static GObjectClass *parent_class = NULL;

static void
impl_manager_do_command (EphyCommandManager *manager,
			 const char *command) 
{
}

static gboolean
impl_manager_can_do_command (EphyCommandManager *manager,
			     const char *command) 
{
}

static void
ephy_command_manager_iface_init (EphyCommandManagerIface *iface)
{
	iface->do_command = impl_manager_do_command;
	iface->can_do_command = impl_manager_can_do_command;
}
	
GType 
webcore_embed_get_type (void)
{
        static GType webcore_embed_type = 0;

        if (webcore_embed_type == 0)
        {
                static const GTypeInfo our_info =
                {
                        sizeof (WebcoreEmbedClass),
                        NULL, /* base_init */
                        NULL, /* base_finalize */
                        (GClassInitFunc) webcore_embed_class_init,
                        NULL,
                        NULL, /* class_data */
                        sizeof (WebcoreEmbed),
                        0, /* n_preallocs */
                        (GInstanceInitFunc) webcore_embed_init
                };

		static const GInterfaceInfo embed_info =
		{
			(GInterfaceInitFunc) ephy_embed_iface_init,
        		NULL,
        		NULL
     		};

		static const GInterfaceInfo ephy_command_manager_info =
		{
			(GInterfaceInitFunc) ephy_command_manager_iface_init,
        		NULL,
        		NULL
     		 };
	
                webcore_embed_type = g_type_register_static (GTK_TYPE_BIN,
							     "WebcoreEmbed",
							     &our_info, 
							     (GTypeFlags)0);
		g_type_add_interface_static (webcore_embed_type,
                                   	     EPHY_TYPE_EMBED,
                                   	     &embed_info);
		g_type_add_interface_static (webcore_embed_type,
                                   	     EPHY_TYPE_COMMAND_MANAGER,
                                   	     &ephy_command_manager_info);
        }

        return webcore_embed_type;
}

static gboolean
impl_find_next (EphyEmbed *embed, 
                gboolean backwards)
{
	return FALSE;
}

static void
impl_activate (EphyEmbed *embed) 
{
	gtk_widget_grab_focus (GTK_BIN (embed)->child);
}

static void
impl_find_set_properties (EphyEmbed *embed, 
                          const char *search_string,
	                  gboolean case_sensitive,
			  gboolean wrap_around)
{
}

static void
embed_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
        GtkWidget *child;

        widget->allocation = *allocation;
        child = GTK_BIN (widget)->child;

        if (child && GTK_WIDGET_VISIBLE (child))
        {
                gtk_widget_size_allocate (child, allocation);
        }
}

static void
webcore_embed_class_init (WebcoreEmbedClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
     	GtkObjectClass *gtk_object_class = GTK_OBJECT_CLASS (klass); 
     	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass); 

	parent_class = (GObjectClass *) g_type_class_peek_parent (klass);

	object_class->finalize = webcore_embed_finalize;

	gtk_object_class->destroy = webcore_embed_destroy;

	widget_class->size_allocate = embed_size_allocate;

	g_type_class_add_private (object_class, sizeof(WebcoreEmbedPrivate));
}

static void
webcore_embed_init (WebcoreEmbed *embed)
{
	GtkWidget *khtml;
        embed->priv = WEBCORE_EMBED_GET_PRIVATE (embed);
	embed->priv->security_state = STATE_IS_UNKNOWN;

	khtml = gtk_khtml_new ();
	gtk_widget_show (khtml);
        gtk_container_add (GTK_CONTAINER (embed), khtml);

	g_signal_connect_object (G_OBJECT (khtml), "location",
				 G_CALLBACK (webcore_embed_location_changed_cb),
				 embed, (GConnectFlags) 0);
	g_signal_connect_object (G_OBJECT (khtml), "net_state_all",
				 G_CALLBACK (webcore_embed_net_state_all_cb),
				 embed, (GConnectFlags) 0);
	g_signal_connect_object (G_OBJECT (khtml), "dom_mouse_click",
				 G_CALLBACK (webcore_embed_dom_mouse_click_cb),
				 embed, (GConnectFlags) 0);
	g_signal_connect_object (G_OBJECT (khtml), "dom_mouse_down",
				 G_CALLBACK (webcore_embed_dom_mouse_down_cb),
				 embed, (GConnectFlags) 0);
	g_signal_connect_object (G_OBJECT (khtml), "new_window",
				 G_CALLBACK (webcore_embed_new_window_cb),
				 embed, (GConnectFlags) 0);
	g_signal_connect_object (G_OBJECT (khtml), "security_change",
				 G_CALLBACK (webcore_embed_security_change_cb),
				 embed, (GConnectFlags) 0);
	g_signal_connect_object (G_OBJECT (khtml), "dom_key_down",
				 G_CALLBACK (webcore_embed_dom_key_down_cb),
				 embed, (GConnectFlags) 0);
}

static void
webcore_embed_destroy (GtkObject *object)
{
	WebcoreEmbed *embed = WEBCORE_EMBED (object);
	GTK_OBJECT_CLASS (parent_class)->destroy (object);
}

static void
webcore_embed_finalize (GObject *object)
{
	WebcoreEmbed *embed = WEBCORE_EMBED (object);
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
impl_load_url (EphyEmbed *embed, 
               const char *url)
{
	gtk_khtml_load_url (GTK_KHTML (embed), url);
}

static void
impl_stop_load (EphyEmbed *embed)
{
	gtk_khtml_stop_load (GTK_KHTML (embed));
}

static gboolean
impl_can_go_back (EphyEmbed *embed)
{
	return gtk_khtml_can_go_back (GTK_KHTML (embed));
}

static gboolean
impl_can_go_forward (EphyEmbed *embed)
{
	return gtk_khtml_can_go_forward (GTK_KHTML (embed));
}

static gboolean
impl_can_go_up (EphyEmbed *embed)
{
	return FALSE;
}

static GSList *
impl_get_go_up_list (EphyEmbed *embed)
{
	return FALSE;
}

static void
impl_go_back (EphyEmbed *embed)
{
	gtk_khtml_go_back (GTK_KHTML (embed));
}
		
static void
impl_go_forward (EphyEmbed *embed)
{
	gtk_khtml_go_forward (GTK_KHTML (embed));
}

static void
impl_go_up (EphyEmbed *embed)
{
}

static char *
impl_get_title (EphyEmbed *embed)
{
	return g_strdup (gtk_khtml_get_title (GTK_KHTML (embed)));
}

static char *
impl_get_link_message (EphyEmbed *embed)
{
}

static char *
impl_get_js_status (EphyEmbed *embed)
{
}

static char *
impl_get_location (EphyEmbed *embed, 
                   gboolean toplevel)
{
	return g_strdup (gtk_khtml_get_location (GTK_KHTML (embed)));
}

static void
impl_reload (EphyEmbed *embed, 
             gboolean force)
{
	gtk_khtml_refresh (GTK_KHTML (embed));
}

static void
impl_set_zoom (EphyEmbed *embed, 
               float zoom) 
{
	g_return_if_fail (zoom > 0.0);

	gtk_khtml_set_text_multiplier (GTK_KHTML (embed), zoom);
	g_signal_emit_by_name (embed, "ge_zoom_change", zoom);
}

static float
impl_get_zoom (EphyEmbed *embed)
{
	return gtk_khtml_get_text_multiplier (GTK_KHTML (embed));
}

static int
impl_shistory_n_items (EphyEmbed *embed)
{
	return -1;
}

static void
impl_shistory_get_nth (EphyEmbed *embed, 
                       int nth,
                       gboolean is_relative,
                       char **aUrl,
                       char **aTitle)
{
}

static int
impl_shistory_get_pos (EphyEmbed *embed)
{
	return -1;
}

static void
impl_shistory_go_nth (EphyEmbed *embed, 
                      int nth)
{
}

static void
impl_get_security_level (EphyEmbed *embed, 
                         EmbedSecurityLevel *level,
                         char **description)
{
	*level = STATE_IS_UNKNOWN;
	*description = NULL;
}

static void
impl_print (EphyEmbed *embed)
{
}

static void
impl_set_print_preview_mode (EphyEmbed *embed, gboolean preview_mode)
{
}

static int
impl_print_preview_n_pages (EphyEmbed *embed)
{
}

static void
impl_print_preview_navigate (EphyEmbed *embed,
			     EmbedPrintPreviewNavType type,
			     int page)
{
}

static void
impl_set_encoding (EphyEmbed *embed,
		   const char *encoding)
{
}

static char *
impl_get_encoding (EphyEmbed *embed)
{
}

static gboolean
impl_has_automatic_encoding (EphyEmbed *embed)
{
	return FALSE;
}

static gboolean
impl_has_modified_forms (EphyEmbed *embed)
{
	return FALSE;
}

static void
webcore_embed_location_changed_cb (GtkKHTML *kembed,
                                   WebcoreEmbed *embed)
{
        const char *location;

        location = gtk_khtml_get_location (kembed);
        g_signal_emit_by_name (embed, "ge_location", location);
}

static void
webcore_embed_net_state_all_cb (GtkKHTML *kembed, const char *URI,
                                gint state, guint status, 
				WebcoreEmbed *membed)
{
}

static gint
webcore_embed_dom_key_down_cb (GtkKHTML *embed, gpointer dom_event,
		               WebcoreEmbed *membed)
{
}

static gint
webcore_embed_dom_mouse_click_cb (GtkKHTML *embed, gpointer dom_event, 
				  WebcoreEmbed *membed)
{
}

static gint
webcore_embed_dom_mouse_down_cb (GtkKHTML *embed, gpointer dom_event, 
				 WebcoreEmbed *membed)
{
}

static void
webcore_embed_new_window_cb (GtkKHTML *embed, 
			     GtkKHTML **newEmbed,
                             guint chrome_mask, 
			     WebcoreEmbed *membed)
{
}

static void
webcore_embed_security_change_cb (GtkKHTML *embed, 
				  gpointer requestptr,
				  guint state, 
				  WebcoreEmbed *membed)
{
}

static EmbedSecurityLevel
webcore_embed_security_level (WebcoreEmbed *membed)
{
}

static void
ephy_embed_iface_init (EphyEmbedIface *iface)
{
	iface->load_url = impl_load_url; 
	iface->stop_load = impl_stop_load;
	iface->can_go_back = impl_can_go_back;
	iface->can_go_forward =impl_can_go_forward;
	iface->can_go_up = impl_can_go_up;
	iface->get_go_up_list = impl_get_go_up_list;
	iface->go_back = impl_go_back;
	iface->go_forward = impl_go_forward;
	iface->go_up = impl_go_up;
	iface->get_title = impl_get_title;
	iface->get_location = impl_get_location;
	iface->get_link_message = impl_get_link_message;
	iface->get_js_status = impl_get_js_status;
	iface->reload = impl_reload;
	iface->set_zoom = impl_set_zoom;
	iface->get_zoom = impl_get_zoom;
	iface->shistory_n_items = impl_shistory_n_items;
	iface->shistory_get_nth = impl_shistory_get_nth;
	iface->shistory_get_pos = impl_shistory_get_pos;
	iface->shistory_go_nth = impl_shistory_go_nth;
	iface->get_security_level = impl_get_security_level;
	iface->find_next = impl_find_next;
	iface->activate = impl_activate;
	iface->find_set_properties = impl_find_set_properties;
	iface->set_encoding = impl_set_encoding;
	iface->get_encoding = impl_get_encoding;
	iface->has_automatic_encoding = impl_has_automatic_encoding;
	iface->print = impl_print;
	iface->set_print_preview_mode = impl_set_print_preview_mode;
	iface->print_preview_n_pages = impl_print_preview_n_pages;
	iface->print_preview_navigate = impl_print_preview_navigate;
	iface->has_modified_forms = impl_has_modified_forms;
}
