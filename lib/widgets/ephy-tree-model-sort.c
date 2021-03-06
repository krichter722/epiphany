/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*  Rhythmbox.
 *  Copyright © 2002 Olivier Martin <omartin@ifrance.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gtk/gtk.h>
#include <string.h>

#include "ephy-node.h"
#include "ephy-tree-model-sort.h"
#include "eggtreemultidnd.h"
#include "ephy-dnd.h"
#include "ephy-debug.h"

/**
 * SECTION:ephy-tree-model-sort
 * @short_description: A #GtkTreeModelSort wrapper
 *
 * #EphyTreeModelSort is a simple wrapper for models, it implements some extra
 * functionalities like drag and dropping, mostly relevant to Epiphany only.
 */

static void ephy_tree_model_sort_finalize (GObject *object);
static void ephy_tree_model_sort_multi_drag_source_interface_init (EggTreeMultiDragSourceIface *iface);
static gboolean ephy_tree_model_sort_multi_row_draggable (EggTreeMultiDragSource *drag_source,
                                                          GList                  *path_list);
static gboolean ephy_tree_model_sort_multi_drag_data_get (EggTreeMultiDragSource *drag_source,
                                                          GList                  *path_list,
                                                          GtkSelectionData       *selection_data);
static gboolean ephy_tree_model_sort_multi_drag_data_delete (EggTreeMultiDragSource *drag_source,
                                                             GList                  *path_list);

struct _EphyTreeModelSort {
  GtkTreeModelSort parent_instance;
  char *str_list;
  int base_drag_column_id;
  int extra_drag_column_id;
};

G_DEFINE_TYPE_WITH_CODE (EphyTreeModelSort, ephy_tree_model_sort, GTK_TYPE_TREE_MODEL_SORT,
                         G_IMPLEMENT_INTERFACE (EGG_TYPE_TREE_MULTI_DRAG_SOURCE,
                                                ephy_tree_model_sort_multi_drag_source_interface_init))

static void
ephy_tree_model_sort_class_init (EphyTreeModelSortClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = ephy_tree_model_sort_finalize;
}

static void
ephy_tree_model_sort_init (EphyTreeModelSort *ma)
{
  ma->base_drag_column_id = -1;
  ma->extra_drag_column_id = -1;
}

static void
ephy_tree_model_sort_finalize (GObject *object)
{
  EphyTreeModelSort *model = EPHY_TREE_MODEL_SORT (object);

  g_free (model->str_list);

  G_OBJECT_CLASS (ephy_tree_model_sort_parent_class)->finalize (object);
}

/**
 * ephy_tree_model_sort_new:
 * @child_model: model to wrap
 *
 * Creates a new #EphyTreeModelSort around @child_model.
 *
 * Returns: a new #EphyTreeModelSort, as a #GtkWidget
 **/
GtkTreeModel *
ephy_tree_model_sort_new (GtkTreeModel *child_model)
{
  GtkTreeModel *model;

  g_return_val_if_fail (child_model != NULL, NULL);

  model = GTK_TREE_MODEL (g_object_new (EPHY_TYPE_TREE_MODEL_SORT,
                                        "model", child_model,
                                        NULL));

  return model;
}

static void
ephy_tree_model_sort_multi_drag_source_interface_init (EggTreeMultiDragSourceIface *iface)
{
  iface->row_draggable = ephy_tree_model_sort_multi_row_draggable;
  iface->drag_data_get = ephy_tree_model_sort_multi_drag_data_get;
  iface->drag_data_delete = ephy_tree_model_sort_multi_drag_data_delete;
}

static gboolean
ephy_tree_model_sort_multi_row_draggable (EggTreeMultiDragSource *drag_source, GList *path_list)
{
  return (EPHY_TREE_MODEL_SORT (drag_source)->base_drag_column_id >= 0);
}

/**
 * ephy_tree_model_sort_set_base_drag_column_id:
 * @ms: an #EphyTreeModelSort
 * @id: base drag column id
 *
 * Sets @id as the column for the base drag data
 **/
void
ephy_tree_model_sort_set_base_drag_column_id (EphyTreeModelSort *ms,
                                              int                id)
{
  ms->base_drag_column_id = id;
}

/**
 * ephy_tree_model_sort_set_extra_drag_column_id:
 * @ms: an #EphyTreeModelSort
 * @id: extra drag column id
 *
 * Sets @id as the column for extra drag data.
 **/
void
ephy_tree_model_sort_set_extra_drag_column_id (EphyTreeModelSort *ms,
                                               int                id)
{
  ms->extra_drag_column_id = id;
}

static gboolean
ephy_tree_model_sort_multi_drag_data_delete (EggTreeMultiDragSource *drag_source,
                                             GList                  *path_list)
{
  return TRUE;
}

static void
each_property_get_data_binder (EphyDragEachSelectedItemDataGet iteratee,
                               gpointer iterator_context, gpointer data)
{
  gpointer *context = (gpointer *)iterator_context;
  GList *path_list = (GList *)(context[0]);
  GList *i;
  EphyTreeModelSort *model = EPHY_TREE_MODEL_SORT (context[1]);
  GValue base_value = { 0, }, extra_value = { 0, };

  for (i = path_list; i != NULL; i = i->next) {
    GtkTreeIter iter;
    GtkTreePath *path = NULL;
    const char *base_data, *extra_data;

    path = gtk_tree_row_reference_get_path (i->data);
    gtk_tree_model_get_iter (GTK_TREE_MODEL (model), &iter, path);

    gtk_tree_model_get_value (GTK_TREE_MODEL (model), &iter,
                              model->base_drag_column_id,
                              &base_value);
    base_data = g_value_get_string (&base_value);

    if (model->extra_drag_column_id >= 0) {
      gtk_tree_model_get_value (GTK_TREE_MODEL (model), &iter,
                                model->extra_drag_column_id,
                                &extra_value);
      extra_data = g_value_get_string (&extra_value);
    } else
      extra_data = NULL;

    g_return_if_fail (base_data != NULL);

    LOG ("Data get %s (%s)", base_data, extra_data);

    iteratee (base_data, extra_data, data);

    gtk_tree_path_free (path);
    g_value_unset (&base_value);

    if (model->extra_drag_column_id >= 0)
      g_value_unset (&extra_value);
  }
}

static gboolean
ephy_tree_model_sort_multi_drag_data_get (EggTreeMultiDragSource *drag_source,
                                          GList                  *path_list,
                                          GtkSelectionData       *selection_data)
{
  gpointer icontext[2];

  icontext[0] = path_list;
  icontext[1] = drag_source;

  ephy_dnd_drag_data_get (NULL, NULL, selection_data,
                          0, &icontext, each_property_get_data_binder);

  return TRUE;
}
