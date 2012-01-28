/*
 *  Copyright © 2005 Christian Persch
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

#include "ephy-action-helper.h"

#define SENSITIVITY_KEY	"EphyAction::Sensitivity"

/**
 * ephy_action_change_sensitivity_flags:
 * @action: a #GtkAction object
 * @flags: arbitrary combination of bit flags, defined by the user
 * @set: %TRUE if @flags should be added to @action
 *
 * This helper function provides an extra layer on top of #GtkAction to
 * manage its sensitivity. It uses bit @flags defined by the user, like
 * in ephy-window.c, SENS_FLAG_*.
 *
 * Effectively, the @action won't be sensitive until it has no flags
 * set. This means you can stack @flags for different events or
 * conditions at the same time.
 */
void 
ephy_action_change_sensitivity_flags (GtkAction *action,
				      guint flags,
				      gboolean set)
{
	static GQuark sensitivity_quark = 0;
	GObject *object = (GObject *) action;
	guint value;

	if (G_UNLIKELY (sensitivity_quark == 0))
	{
		sensitivity_quark = g_quark_from_static_string (SENSITIVITY_KEY);
	}

	value = GPOINTER_TO_UINT (g_object_get_qdata (object, sensitivity_quark));

	if (set)
	{
		value |= flags;
	}
	else
	{
		value &= ~flags;
	}

	g_object_set_qdata (object, sensitivity_quark, GUINT_TO_POINTER (value));

	gtk_action_set_sensitive (GTK_ACTION (action), value == 0);
}

typedef struct {
    int index;
    GBindingTransformFunc transform;
} BindData;

static void
free_bind_data (BindData *data)
{
    g_slice_free (BindData, data);
}

#define BIND_KEY "EphyAction::Bind"

static gboolean
_ephy_action_sensitivity_transform (GBinding *binding,
                                    const GValue *source_value,
                                    GValue *target_value,
                                    gpointer user_data)
{
    BindData *data = (BindData*)user_data;
    static GQuark bind_quark = 0;
    GObject *target;
    guint value;
    gboolean set = FALSE;

    if (G_UNLIKELY (bind_quark == 0))
        bind_quark = g_quark_from_static_string (BIND_KEY);

    g_object_get (binding, "target", &target, NULL);

    value = GPOINTER_TO_UINT (g_object_get_qdata (target, bind_quark));

    if (data->transform)
        set = data->transform (binding,
                               source_value,
                               target_value,
                               user_data);
    else
        set = g_value_get_boolean (source_value);

    if (set)
        value |= data->index;
    else
        value &= ~data->index;

    g_object_set_qdata (target, bind_quark, GUINT_TO_POINTER (value));
    gtk_action_set_sensitive (GTK_ACTION (target), value == 0);

    /* Do nothing else. */
    return FALSE;
}

static void
ephy_action_bind_sensitivity_valist (GtkAction *action,
                                     va_list var_args)
{
    GObject *object;
    int i = 1;

    object = va_arg (var_args, GObject*);

    while (object) {
        const char *property;
        GBindingTransformFunc transform;
        GBindingFlags flags;
        BindData *data;

        property = va_arg (var_args, const char*);
        transform = va_arg (var_args, GBindingTransformFunc);
        flags = va_arg (var_args, GBindingFlags);

        data = g_slice_new (BindData);
        data->index = i;
        data->transform = transform;

        g_object_bind_property_full (object, property,
                                     action, "sensitive",
                                     flags,
                                     _ephy_action_sensitivity_transform,
                                     NULL,
                                     data, (GDestroyNotify)free_bind_data);
        i *= 2;

        object = va_arg (var_args, GObject*);
    }
}

/**
 * ephy_action_bind_sensitivity:
 * @action: a #GtkAction

 * @...: a %NULL terminated list of properties that will control the
 * sensitivity of the @action, as follows: the #GObject that holds the
 * property, its name as C string, an optional #GBindingTransformFunc
 * for the property and optional #GBindingFlags.
 * 
 * This method will set-up a series of property bindings so that
 * @action is only sensitive if all the listed properties are %FALSE
 * (or the defined #GBindingTransformFunc for a given property returns
 * %FALSE).
 *
 **/
void
ephy_action_bind_sensitivity (GtkAction *action,
                              ...)
{
    va_list var_args;

    g_return_if_fail (GTK_IS_ACTION (action));

    va_start (var_args, action);
    ephy_action_bind_sensitivity_valist (action, var_args);
    va_end (var_args);
}
