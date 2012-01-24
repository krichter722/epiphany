/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
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
 *  Author: Claudio Saavedra  <csaavedra@igalia.com>
 */

#include "config.h"
#include "ephy-debug.h"
#include "ephy-snapshot-service.h"

static time_t mtime;

static void
on_snapshot_ready (GdkPixbuf *snapshot,
                   gint data)
{
  g_assert_cmpint (data, ==, G_MAXINT);
  g_assert (GDK_IS_PIXBUF (snapshot));

  gtk_main_quit();
}

static void
test_snapshot (void)
{
  EphySnapshotService *service = ephy_snapshot_service_get_default ();

  ephy_snapshot_service_get_snapshot (service,
                                      "http://www.gnome.org",
                                      mtime,
                                      (EphySnapshotServiceCallback)on_snapshot_ready,
                                      GINT_TO_POINTER (G_MAXINT));
  gtk_main ();
}

static void
test_cached_snapshot (void)
{
  EphySnapshotService *service = ephy_snapshot_service_get_default ();

  ephy_snapshot_service_get_snapshot (service,
                                      "http://www.gnome.org",
                                      mtime,
                                      (EphySnapshotServiceCallback)on_snapshot_ready,
                                      GINT_TO_POINTER (G_MAXINT));
  gtk_main ();
}

int
main (int argc, char *argv[])
{
  gtk_test_init (&argc, &argv);
  ephy_debug_init ();

  mtime = time(NULL);

  g_test_add_func ("/lib/ephy-snapshot-service/test_snapshot",
                   test_snapshot);
  g_test_add_func ("/lib/ephy-snapshot-service/test_cached_snapshot",
                   test_cached_snapshot);

  return g_test_run ();
}
