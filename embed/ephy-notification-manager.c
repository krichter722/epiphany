/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright © 2016 Gabriel Ivascu <ivascu.gabriel59@gmail.com>
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
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include "ephy-notification-manager.h"

struct _EphyNotificationManager {
  GdNotification  parent_instance;

  GtkWidget      *grid;
};

struct _EphyNotificationManagerClass {
  GdNotificationClass parent_class;
};

G_DEFINE_TYPE (EphyNotificationManager, ephy_notification_manager, GD_TYPE_NOTIFICATION);

static EphyNotificationManager *notification_manager = NULL;

static void
ephy_notification_manager_init (EphyNotificationManager *self)
{
  /* Globally accessible singleton */
  g_assert (notification_manager == NULL);
  notification_manager = self;
  g_object_add_weak_pointer (G_OBJECT (notification_manager), (gpointer *)&notification_manager);

  gtk_widget_set_halign (GTK_WIDGET (self), GTK_ALIGN_CENTER);
  gtk_widget_set_valign (GTK_WIDGET (self), GTK_ALIGN_START);

  self->grid = gtk_grid_new ();
  gtk_orientable_set_orientation (GTK_ORIENTABLE (self->grid), GTK_ORIENTATION_VERTICAL);
  gtk_grid_set_row_spacing (GTK_GRID (self->grid), 6);
  gtk_container_add (GTK_CONTAINER (self), self->grid);
}

static void
ephy_notification_manager_class_init (EphyNotificationManagerClass *klass)
{
}

EphyNotificationManager *
ephy_notification_manager_get_default (void)
{
  if (notification_manager != NULL)
    return notification_manager;

  return g_object_new (EPHY_TYPE_NOTIFICATION_MANAGER,
                       "show-close-button", TRUE,
                       "timeout", -1,
                       NULL);
}

void
ephy_notification_manager_add_notification (EphyNotificationManager *self,
                                            GtkWidget               *notification)
{
  g_return_if_fail (EPHY_IS_NOTIFICATION_MANAGER (self));
  g_return_if_fail (GTK_IS_WIDGET (notification));

  gtk_container_add (GTK_CONTAINER (self->grid), notification);
  gtk_widget_show_all (GTK_WIDGET (self));
}
