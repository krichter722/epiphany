/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright © 2012 Igalia S.L.
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
 */

#include "config.h"

#include "ephy-notebook.h"
#include "ephy-active-store.h"
#include "ephy-snapshot-service.h"

#define EPHY_ACTIVE_STORE_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), EPHY_TYPE_ACTIVE_STORE, EphyActiveStorePrivate))

struct _EphyActiveStorePrivate
{
  EphyNotebook *notebook;
};

enum
{
  PROP_0,
  PROP_NOTEBOOK
};

G_DEFINE_TYPE (EphyActiveStore, ephy_active_store, EPHY_TYPE_OVERVIEW_STORE)

static void
ephy_active_store_set_property (GObject *object,
                                guint prop_id,
                                const GValue *value,
                                GParamSpec *pspec)
{
  EphyActiveStore *store = EPHY_ACTIVE_STORE (object);

  switch (prop_id)
  {
  case PROP_NOTEBOOK:
    ephy_active_store_set_notebook (store, g_value_get_object (value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
ephy_active_store_get_property (GObject *object,
                                guint prop_id,
                                GValue *value,
                                GParamSpec *pspec)
{
  EphyActiveStore *store = EPHY_ACTIVE_STORE (object);

  switch (prop_id)
  {
  case PROP_NOTEBOOK:
    g_value_set_object (value, ephy_active_store_get_notebook (store));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
ephy_active_store_class_init (EphyActiveStoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = ephy_active_store_get_property;
  object_class->set_property = ephy_active_store_set_property;

  g_object_class_install_property (object_class,
                                   PROP_NOTEBOOK,
                                   g_param_spec_object ("notebook",
                                                        "Notebook",
                                                        "The notebook holding the active pages",
                                                        EPHY_TYPE_NOTEBOOK,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

  g_type_class_add_private (object_class, sizeof (EphyActiveStorePrivate));
}

static void
ephy_active_store_init (EphyActiveStore *self)
{
  GType types[EPHY_ACTIVE_STORE_NCOLS];

  self->priv = EPHY_ACTIVE_STORE_GET_PRIVATE (self);

  types[EPHY_OVERVIEW_STORE_ID] = G_TYPE_STRING;
  types[EPHY_OVERVIEW_STORE_URI] = G_TYPE_STRING;
  types[EPHY_OVERVIEW_STORE_TITLE] = G_TYPE_STRING;
  types[EPHY_OVERVIEW_STORE_AUTHOR] = G_TYPE_STRING;
  types[EPHY_OVERVIEW_STORE_SNAPSHOT] = GDK_TYPE_PIXBUF;
  types[EPHY_OVERVIEW_STORE_LAST_VISIT] = G_TYPE_LONG;
  types[EPHY_OVERVIEW_STORE_SELECTED] = G_TYPE_BOOLEAN;
  types[EPHY_OVERVIEW_STORE_SNAPSHOT_CANCELLABLE] = G_TYPE_CANCELLABLE;
  types[EPHY_ACTIVE_STORE_TAB_POS] = G_TYPE_UINT;

  gtk_list_store_set_column_types (GTK_LIST_STORE (self),
                                   EPHY_ACTIVE_STORE_NCOLS, types);

  gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (self),
                                        EPHY_ACTIVE_STORE_TAB_POS,
                                        GTK_SORT_ASCENDING);
}

typedef void (* BindingFunc) (GtkTreeModel *model, GtkTreeIter *iter, GValue *value, gpointer data);

typedef struct
{
  GtkTreeModel *model;
  const char *property_name;
  GtkTreeRowReference *row_ref;
  guint column;
  gboolean is_child_property;
  BindingFunc func;
  gpointer data;
} Binding;

static void
on_object_notify (GObject *object,
                  GParamSpec *pspec,
                  Binding* binding)
{
  GValue value = G_VALUE_INIT;
  GType type;
  GtkTreeIter iter;
  GtkTreePath *path;
  const char *p_name;

  p_name = g_intern_string (pspec->name);

  if (p_name != binding->property_name)
    return;

  type = gtk_tree_model_get_column_type (binding->model,
                                         binding->column);
  g_value_init (&value, type);

  if (binding->is_child_property) {
    GtkWidget *widget = GTK_WIDGET (object);
    GtkContainer *container = GTK_CONTAINER (gtk_widget_get_parent (widget));

    gtk_container_child_get_property (container, widget,
                                      binding->property_name, &value);
  } else {
    g_object_get_property (object, binding->property_name, &value);
  }
  path = gtk_tree_row_reference_get_path (binding->row_ref);
  gtk_tree_model_get_iter (binding->model, &iter, path);
  gtk_tree_path_free (path);

  gtk_list_store_set_value (GTK_LIST_STORE (binding->model), &iter,
                            binding->column, &value);
  if (binding->func)
    (*binding->func) (binding->model, &iter, &value, binding->data);
}

static Binding *
binding_new (GtkTreeModel *model,
             const char *property,
             guint column,
             BindingFunc func,
             gpointer data,
             gboolean is_child_property,
             GtkTreeIter *iter)
{
  GtkTreePath *path;
  Binding* binding = g_slice_new0 (Binding);

  binding->model = model;
  binding->property_name = g_intern_string (property);
  binding->column = column;
  binding->func = func;
  binding->data = data;
  binding->is_child_property = is_child_property;

  path = gtk_tree_model_get_path (model, iter);
  binding->row_ref = gtk_tree_row_reference_new (model, path);
  gtk_tree_path_free (path);

  return binding;
}

static void
binding_free (Binding *binding)
{
  gtk_tree_row_reference_free (binding->row_ref);
  g_slice_free (Binding, binding);
}

static void
bind_object_property_with_model_row_full (GtkTreeModel *model,
                                          GtkTreeIter *iter,
                                          guint column,
                                          GObject *object,
                                          const char *property,
                                          gboolean is_child_property,
                                          BindingFunc func,
                                          gpointer data)
{
  GParamSpec *pspec;
  Binding *binding;

  binding = binding_new (model, property, column, func,
                         data, is_child_property, iter);

  g_signal_connect_data (object, is_child_property ? "child-notify" : "notify",
                         G_CALLBACK (on_object_notify), binding,
                         (GClosureNotify)binding_free, 0);

  if (is_child_property)
    pspec = gtk_container_class_find_child_property (G_OBJECT_GET_CLASS (object),
                                                     property);
  else
    pspec = g_object_class_find_property (G_OBJECT_GET_CLASS (object),
                                          property);

  on_object_notify (object, pspec, binding);
}

static void
bind_object_property_with_model_row (GtkTreeModel *model,
                                     GtkTreeIter *iter,
                                     guint column,
                                     GObject *object,
                                     const gchar *property)
{
  bind_object_property_with_model_row_full (model, iter, column,
                                            object, property, FALSE,
                                            NULL, NULL);
}

static void
webview_uri_changed (GtkTreeModel *model,
                     GtkTreeIter *iter,
                     GValue *value,
                     WebKitWebView *webview)
{
  ephy_overview_store_peek_snapshot (EPHY_OVERVIEW_STORE (model), webview, iter);
}

#ifdef HAVE_WEBKIT2
static void
webview_load_changed_cb (WebKitWebView *webview,
                         WebKitLoadEvent load_event,
                         Binding *binding)
{
  if (load_event == WEBKIT_LOAD_FINISHED) {
    on_object_notify (G_OBJECT (webview),
                      g_object_class_find_property (G_OBJECT_GET_CLASS (webview), "uri"),
                      binding);
  }
}
#endif

static void
ephy_active_store_add_embed (EphyActiveStore *store,
                             EphyEmbed *embed,
                             gint position)
{
  GtkTreeIter iter;
  EphyWebView *webview;
#ifdef HAVE_WEBKIT2
  Binding *binding;
#endif
  webview = ephy_embed_get_web_view (embed);

  gtk_list_store_insert (GTK_LIST_STORE (store), &iter, position);
#ifdef HAVE_WEBKIT2
  /* WebKit2 changes the URI more often than we'd like. Since we're
     only interested in URLs that are actually being loaded, we hook
     the binding to the "load-changed" signal, and selectively call
     on_object_notify(). */
  binding = binding_new (GTK_TREE_MODEL (store), "uri", EPHY_OVERVIEW_STORE_URI,
                         (BindingFunc)webview_uri_changed, webview, FALSE, &iter);
  g_signal_connect (webview, "load-changed",
                    G_CALLBACK (webview_load_changed_cb), binding);
  on_object_notify (G_OBJECT (webview),
                    g_object_class_find_property (G_OBJECT_GET_CLASS (webview), "uri"),
                    binding);
#else
  bind_object_property_with_model_row_full (GTK_TREE_MODEL (store), &iter,
                                            EPHY_OVERVIEW_STORE_URI,
                                            G_OBJECT (webview), "uri", FALSE,
                                            (BindingFunc)webview_uri_changed, webview);
#endif

  bind_object_property_with_model_row (GTK_TREE_MODEL (store), &iter,
                                       EPHY_OVERVIEW_STORE_TITLE,
                                       G_OBJECT (webview), "title");
  bind_object_property_with_model_row_full (GTK_TREE_MODEL (store), &iter,
                                            EPHY_ACTIVE_STORE_TAB_POS,
                                            G_OBJECT (embed), "position", TRUE,
                                            NULL, NULL);

}

static void
bindings_drop (GObject *object)
{
  g_signal_handlers_disconnect_matched (object,
                                        G_SIGNAL_MATCH_FUNC,
                                        0, 0, NULL, on_object_notify,
                                        NULL);
}

typedef struct {
  GtkTreeIter iter;
  gint position;
} HelperCtx;

static gboolean
foreach_helper (GtkTreeModel *model,
                GtkTreePath *path,
                GtkTreeIter *iter,
                HelperCtx *ctx)
{
  gint position;

  gtk_tree_model_get (model, iter,
                      EPHY_ACTIVE_STORE_TAB_POS, &position, -1);
  if (ctx->position == position) {
    ctx->iter = *iter;
    return TRUE;
  }
  return FALSE;
}

static void
ephy_active_store_remove_embed (EphyActiveStore *store,
                                gint position,
                                EphyEmbed *embed)
{
  GtkTreeIter iter;
  EphyWebView *webview;
  HelperCtx *ctx;

  webview = ephy_embed_get_web_view (embed);
  ctx = g_slice_new0 (HelperCtx);
  ctx->position = position;
  gtk_tree_model_foreach (GTK_TREE_MODEL (store),
                          (GtkTreeModelForeachFunc) foreach_helper,
                          ctx);
  iter = ctx->iter;
  g_slice_free (HelperCtx, ctx);

  ephy_overview_store_remove (EPHY_OVERVIEW_STORE (store), &iter);

  bindings_drop (G_OBJECT (webview));
  bindings_drop (G_OBJECT (embed));

#ifdef HAVE_WEBKIT2
  g_signal_handlers_disconnect_matched (webview,
                                        G_SIGNAL_MATCH_FUNC,
                                        0, 0, NULL, webview_load_changed_cb,
                                        NULL);
#endif
}

static void
notebook_page_added_cb (EphyNotebook *notebook,
                        EphyEmbed *embed,
                        guint position,
                        EphyActiveStore *store)
{
  ephy_active_store_add_embed (store, embed, position);
}

static void
notebook_page_removed_cb (EphyNotebook *notebook,
                          EphyEmbed *embed,
                          guint position,
                          EphyActiveStore *store)
{
  ephy_active_store_remove_embed (store, position, embed);
}

static void
ephy_active_store_populate_helper (EphyEmbed *widget,
                                   EphyActiveStore *store)
{
  ephy_active_store_add_embed (store, widget, G_MAXINT);
}

static void
ephy_active_store_populate (EphyActiveStore *store)
{
  gtk_container_foreach (GTK_CONTAINER (store->priv->notebook),
                         (GtkCallback) ephy_active_store_populate_helper,
                         store);
}

void
ephy_active_store_set_notebook (EphyActiveStore *store,
                                EphyNotebook *notebook)
{
  EphyActiveStorePrivate *priv;

  g_return_if_fail (EPHY_IS_ACTIVE_STORE (store));
  g_return_if_fail (EPHY_IS_NOTEBOOK (notebook));

  if (store->priv->notebook == notebook)
    return;

  priv = store->priv;

  if (priv->notebook) {
    g_signal_handlers_disconnect_by_func (priv->notebook,
                                          notebook_page_added_cb,
                                          store);
    g_signal_handlers_disconnect_by_func (priv->notebook,
                                          notebook_page_removed_cb,
                                          store);
    g_object_unref (priv->notebook);
    gtk_list_store_clear (GTK_LIST_STORE (store));
  }

  priv->notebook = g_object_ref (notebook);

  g_signal_connect (notebook, "page-added",
                    G_CALLBACK (notebook_page_added_cb), store);
  g_signal_connect (notebook, "page-removed",
                    G_CALLBACK (notebook_page_removed_cb), store);
  ephy_active_store_populate (store);
}

EphyNotebook *
ephy_active_store_get_notebook (EphyActiveStore *store)
{
  g_return_val_if_fail (EPHY_IS_ACTIVE_STORE (store), NULL);

  return store->priv->notebook;
}

EphyActiveStore *
ephy_active_store_new (EphyNotebook *notebook)
{
  return g_object_new (EPHY_TYPE_ACTIVE_STORE,
                       "notebook", notebook,
                       NULL);
}
