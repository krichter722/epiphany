/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * test-ephy-action-helper.c
 * This file is part of Epiphany
 *
 * Copyright © 2012 - Igalia S.L.
 *
 * Epiphany is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Epiphany is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Epiphany; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "config.h"

#include "ephy-action-helper.h"
#include "ephy-debug.h"
#include "ephy-embed-prefs.h"
#include "ephy-file-helpers.h"
#include "ephy-shell.h"

#include <gtk/gtk.h>

/* Test source type. */
typedef struct _EphySourcePrivate {
  gboolean foo;
  gboolean bar;
} EphySourcePrivate;

enum {
  PROP_0,
  PROP_FOO,
  PROP_BAR
};

#define EPHY_TYPE_SOURCE (ephy_source_get_type ())
#define EPHY_SOURCE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), EPHY_TYPE_SOURCE, EphySource))

typedef struct {
  GObject parent;

  EphySourcePrivate *priv;
} EphySource;

typedef struct {
  GObjectClass parent_class;
} EphySourceClass;

GType ephy_source_get_type (void) G_GNUC_CONST;

G_DEFINE_TYPE (EphySource, ephy_source, G_TYPE_OBJECT)

static void
ephy_source_get_property (GObject    *object,
                          guint       property_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
  EphySource *source = EPHY_SOURCE (object);

  switch (property_id)
    {
    case PROP_FOO:
      g_value_set_boolean (value, source->priv->foo);
      break;
    case PROP_BAR:
      g_value_set_boolean (value, source->priv->bar);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
ephy_source_set_property (GObject      *object,
                          guint         property_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
  EphySource *source = EPHY_SOURCE (object);

  switch (property_id)
  {
  case PROP_FOO:
    source->priv->foo = g_value_get_boolean (value);
    break;
  case PROP_BAR:
    source->priv->bar = g_value_get_boolean (value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
ephy_source_class_init (EphySourceClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = ephy_source_get_property;
  object_class->set_property = ephy_source_set_property;

  g_object_class_install_property (object_class,
                                   PROP_FOO,
                                   g_param_spec_boolean ("foo", NULL, NULL,
                                                         FALSE,
                                                         G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_CONSTRUCT));

  g_object_class_install_property (object_class,
                                   PROP_BAR,
                                   g_param_spec_boolean ("bar", NULL, NULL,
                                                         FALSE,
                                                         G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_CONSTRUCT));

  g_type_class_add_private (klass, sizeof (EphySourcePrivate));
}

static void
ephy_source_init (EphySource *source)
{
  source->priv = G_TYPE_INSTANCE_GET_PRIVATE (source, EPHY_TYPE_SOURCE, EphySourcePrivate);
}

/* End of test source type definition. */

static gboolean
transform (GBinding *binding,
           const GValue *source_value,
           GValue *target_value,
           gpointer user_data)
{
  return g_value_get_boolean (source_value);
}

static void
test_ephy_action_helper_single_bind ()
{
  GtkAction *action;
  GObject *source;
    
  action = gtk_action_new ("TestAction", NULL, NULL, NULL);
  source = g_object_new (EPHY_TYPE_SOURCE,
                         "foo", TRUE,
                         NULL);

  g_assert (gtk_action_get_sensitive (action) == TRUE);
  ephy_action_bind_sensitivity (action,
                                source, "foo", NULL, G_BINDING_SYNC_CREATE,
                                NULL);
  g_assert (gtk_action_get_sensitive (action) == FALSE);

  g_object_set (source, "foo", FALSE, NULL);
  g_assert (gtk_action_get_sensitive (action) == TRUE);

  g_object_set (source, "foo", TRUE, NULL);
  g_assert (gtk_action_get_sensitive (action) == FALSE);
}

static void
test_ephy_action_helper_double_bind ()
{
  GtkAction *action;
  GObject *source;
    
  action = gtk_action_new ("TestAction", NULL, NULL, NULL);
  source = g_object_new (EPHY_TYPE_SOURCE,
                         "foo", FALSE,
                         "bar", FALSE,
                         NULL);

  g_assert (gtk_action_get_sensitive (action) == TRUE);
  ephy_action_bind_sensitivity (action,
                                source, "foo", NULL, G_BINDING_SYNC_CREATE,
                                source, "bar", NULL, G_BINDING_SYNC_CREATE,
                                NULL);
  g_assert (gtk_action_get_sensitive (action) == TRUE);

  g_object_set (source, "foo", TRUE, NULL);
  g_assert (gtk_action_get_sensitive (action) == FALSE);

  g_object_set (source, "bar", TRUE, NULL);
  g_assert (gtk_action_get_sensitive (action) == FALSE);

  g_object_set (source, "foo", FALSE, NULL);
  g_assert (gtk_action_get_sensitive (action) == FALSE);

  g_object_set (source, "bar", FALSE, NULL);
  g_assert (gtk_action_get_sensitive (action) == TRUE);
}

static void
test_ephy_action_helper_double_bind_transform ()
{
  GtkAction *action;
  GObject *source;
    
  action = gtk_action_new ("TestAction", NULL, NULL, NULL);
  source = g_object_new (EPHY_TYPE_SOURCE,
                         "foo", FALSE,
                         "bar", FALSE,
                         NULL);

  g_assert (gtk_action_get_sensitive (action) == TRUE);
  ephy_action_bind_sensitivity (action,
                                source, "foo", transform, G_BINDING_SYNC_CREATE,
                                source, "bar", transform, G_BINDING_SYNC_CREATE,
                                NULL);
  g_assert (gtk_action_get_sensitive (action) == TRUE);

  g_object_set (source, "foo", TRUE, NULL);
  g_assert (gtk_action_get_sensitive (action) == FALSE);

  g_object_set (source, "bar", TRUE, NULL);
  g_assert (gtk_action_get_sensitive (action) == FALSE);

  g_object_set (source, "foo", FALSE, NULL);
  g_assert (gtk_action_get_sensitive (action) == FALSE);

  g_object_set (source, "bar", FALSE, NULL);
  g_assert (gtk_action_get_sensitive (action) == TRUE);
}

int
main (int argc, char *argv[])
{
  int ret;

  gtk_test_init (&argc, &argv);

  ephy_debug_init ();
  ephy_embed_prefs_init ();

  if (!ephy_file_helpers_init (NULL, TRUE, FALSE, NULL)) {
    g_debug ("Something wrong happened with ephy_file_helpers_init()");
    return -1;
  }

  g_test_add_func ("/src/ephy-action-helper/single-bind",
                   test_ephy_action_helper_single_bind);
  g_test_add_func ("/src/ephy-action-helper/double-bind",
                   test_ephy_action_helper_double_bind);
  g_test_add_func ("/src/ephy-action-helper/double-bind-transform",
                   test_ephy_action_helper_double_bind_transform);

  ret = g_test_run ();

  ephy_file_helpers_shutdown ();

  return ret;
}
