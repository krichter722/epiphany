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
#include <config.h>
#endif

#include "downloader-view.h"
#include "ephy-gui.h"
#include "ephy-prefs.h"
#include "ephy-ellipsizing-label.h"
#include "ephy-embed-utils.h"
#include "ephy-file-helpers.h"
#include "ephy-embed-shell.h"
#include "ephy-stock-icons.h"

#include <gtk/gtktreeview.h>
#include <gtk/gtkliststore.h>
#include <gtk/gtktreeviewcolumn.h>
#include <bonobo/bonobo-i18n.h>
#include <libgnomeui/gnome-dialog-util.h>
#include <libgnomeui/gnome-dialog.h>
#include <gtk/gtkprogressbar.h>
#include <libgnomevfs/gnome-vfs-mime-handlers.h>

#define CONF_DOWNLOADING_SHOW_DETAILS "/apps/epiphany/dialogs/downloader_show_details"

enum
{
	COL_PERCENT,
	COL_FILENAME,
	COL_SIZE,
	COL_REMAINING,
	COL_PERSIST_OBJECT
};

#define EPHY_DOWNLOADER_VIEW_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), EPHY_TYPE_DOWNLOADER_VIEW, DownloaderViewPrivate))

struct DownloaderViewPrivate
{
	GHashTable *details_hash;
	GtkTreeModel *model;
	gboolean show_details;

	/* Widgets */
	GtkWidget *window;
	GtkWidget *treeview;
	GtkWidget *details_location;
	GtkWidget *details_status;
	GtkWidget *details_elapsed;
	GtkWidget *details_remaining;
	GtkWidget *details_progress;
	GtkWidget *details_button;

	GtkWidget *open_button;
	GtkWidget *pause_button;
	GtkWidget *abort_button;
};

typedef struct
{
	GtkTreeRowReference *ref;
} DownloadDetails;

typedef struct
{
	gboolean is_paused;
	gboolean can_abort;
	gboolean can_open;
	gboolean can_pause;
	DownloaderViewPrivate *priv;
} ControlsInfo;

enum
{
	PROP_WINDOW,
	PROP_TREEVIEW,
	PROP_DETAILS_FRAME,
	PROP_DETAILS_TABLE,
	PROP_DETAILS_STATUS,
	PROP_DETAILS_ELAPSED,
	PROP_DETAILS_REMAINING,
	PROP_DETAILS_PROGRESS,
	PROP_OPEN_BUTTON,
	PROP_PAUSE_BUTTON,
	PROP_ABORT_BUTTON,
	PROP_DETAILS_BUTTON
};

static const
EphyDialogProperty properties [] =
{
	{ PROP_WINDOW, "download_manager_dialog", NULL, PT_NORMAL, NULL},
        { PROP_TREEVIEW, "clist", NULL, PT_NORMAL, NULL },
	{ PROP_DETAILS_FRAME, "details_frame", NULL, PT_NORMAL, NULL },
	{ PROP_DETAILS_TABLE, "details_table", NULL, PT_NORMAL, NULL },
	{ PROP_DETAILS_STATUS, "details_status", NULL, PT_NORMAL, NULL },
	{ PROP_DETAILS_ELAPSED, "details_elapsed", NULL, PT_NORMAL, NULL },
	{ PROP_DETAILS_REMAINING, "details_remaining", NULL, PT_NORMAL, NULL },
	{ PROP_DETAILS_PROGRESS, "details_progress", NULL, PT_NORMAL, NULL },
	{ PROP_OPEN_BUTTON, "open_button", NULL, PT_NORMAL, NULL },
	{ PROP_PAUSE_BUTTON, "pause_button", NULL, PT_NORMAL, NULL },
	{ PROP_ABORT_BUTTON, "abort_button", NULL, PT_NORMAL, NULL },
	{ PROP_DETAILS_BUTTON, "details_togglebutton", CONF_DOWNLOADING_SHOW_DETAILS, PT_NORMAL, NULL },

        { -1, NULL, NULL }
};

static void
downloader_view_build_ui (DownloaderView *dv);
static void
downloader_view_class_init (DownloaderViewClass *klass);
static void
downloader_view_finalize (GObject *object);
static void
downloader_view_init (DownloaderView *dv);

/* Callbacks */
void
download_dialog_pause_cb (GtkButton *button, DownloaderView *dv);
void
download_dialog_abort_cb (GtkButton *button, DownloaderView *dv);
static void
downloader_treeview_selection_changed_cb (GtkTreeSelection *selection,
					  DownloaderView *dv);
gboolean
download_dialog_delete_cb (GtkWidget *window, GdkEventAny *event,
			   DownloaderView *dv);
void
download_dialog_details_cb (GtkToggleButton *button,
			    DownloaderView *dv);
void
download_dialog_open_cb (GtkWidget *button,
			 DownloaderView *dv);

static GObjectClass *parent_class = NULL;

GType
downloader_view_get_type (void)
{
       static GType downloader_view_type = 0;

        if (downloader_view_type == 0)
        {
                static const GTypeInfo our_info =
                {
                        sizeof (DownloaderViewClass),
                        NULL, /* base_init */
                        NULL, /* base_finalize */
                        (GClassInitFunc) downloader_view_class_init,
                        NULL, /* class_finalize */
                        NULL, /* class_data */
                        sizeof (DownloaderView),
                        0,    /* n_preallocs */
                        (GInstanceInitFunc) downloader_view_init
                };

                downloader_view_type = g_type_register_static (EPHY_TYPE_DIALOG,
                                                               "DownloaderView",
                                                               &our_info, 0);
        }

        return downloader_view_type;
}

static void
downloader_view_class_init (DownloaderViewClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

        parent_class = g_type_class_peek_parent (klass);

        object_class->finalize = downloader_view_finalize;

	g_type_class_add_private (object_class, sizeof(DownloaderViewPrivate));
}

static void
destroy_details_cb (DownloadDetails *details)
{
	/* FIXME Free row ref */
}

static void
downloader_view_init (DownloaderView *dv)
{
        dv->priv = EPHY_DOWNLOADER_VIEW_GET_PRIVATE (dv);

	dv->priv->details_hash = g_hash_table_new_full (g_direct_hash,
							g_direct_equal,
							NULL,
							(GDestroyNotify)destroy_details_cb);
	downloader_view_build_ui (dv);

	g_object_ref (embed_shell);
}

static void
downloader_view_finalize (GObject *object)
{
        DownloaderView *dv = EPHY_DOWNLOADER_VIEW (object);

	g_hash_table_destroy (dv->priv->details_hash);

	g_object_unref (embed_shell);

        G_OBJECT_CLASS (parent_class)->finalize (object);
}

DownloaderView *
downloader_view_new (void)
{
	return EPHY_DOWNLOADER_VIEW (g_object_new
			(EPHY_TYPE_DOWNLOADER_VIEW, NULL));
}

static void
download_changed_cb (EphyDownload *download, DownloaderView *dv)
{
	DownloadDetails *details;
	GtkTreePath *path;
	GtkTreeIter iter;
	int percent;

	details = g_hash_table_lookup (dv->priv->details_hash,
				       download);
	g_return_if_fail (details != NULL);

	percent = ephy_download_get_percent (download);
	path = gtk_tree_row_reference_get_path (details->ref);
	gtk_tree_model_get_iter (dv->priv->model, &iter, path);
	gtk_list_store_set (GTK_LIST_STORE (dv->priv->model),
			    &iter,
			    COL_PERCENT, percent,
			    -1);
	gtk_tree_path_free (path);
}

void
downloader_view_add_download (DownloaderView *dv,
			      EphyDownload *download)
{
	GtkTreeIter iter;
	DownloadDetails *details;
	GtkTreeSelection *selection;
	GtkTreePath *path;
	char *name;

	details = g_new0 (DownloadDetails, 1);

	g_hash_table_insert (dv->priv->details_hash,
			     g_object_ref (download),
			     details);

	gtk_list_store_append (GTK_LIST_STORE (dv->priv->model),
			       &iter);

	path =  gtk_tree_model_get_path (GTK_TREE_MODEL (dv->priv->model), &iter);
	details->ref = gtk_tree_row_reference_new
				(GTK_TREE_MODEL (dv->priv->model),path);
	gtk_tree_path_free (path);

	name = ephy_download_get_name (download);
	gtk_list_store_set (GTK_LIST_STORE (dv->priv->model),
			    &iter,
			    COL_FILENAME, name,
			    COL_PERSIST_OBJECT, download,
			    -1);
	g_free (name);

	selection = gtk_tree_view_get_selection
		(GTK_TREE_VIEW(dv->priv->treeview));
	gtk_tree_selection_unselect_all (selection);
	gtk_tree_selection_select_iter (selection, &iter);

	g_signal_connect (download, "changed",
			  G_CALLBACK (download_changed_cb), dv);

	ephy_dialog_show (EPHY_DIALOG (dv));
}

static void
downloader_view_build_ui (DownloaderView *dv)
{
	DownloaderViewPrivate *priv = dv->priv;
	GtkListStore *liststore;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
	GtkWidget *details_table;
	GdkPixbuf *icon;
	EphyDialog *d = EPHY_DIALOG (dv);

	ephy_dialog_construct (d,
                               properties,
                               "epiphany.glade",
                               "download_manager_dialog");

	/* lookup needed widgets */
	priv->window = ephy_dialog_get_control(d, PROP_WINDOW);
	priv->treeview = ephy_dialog_get_control (d, PROP_TREEVIEW);
	priv->details_status = ephy_dialog_get_control (d, PROP_DETAILS_STATUS);
	priv->details_elapsed = ephy_dialog_get_control (d, PROP_DETAILS_ELAPSED);
	priv->details_remaining = ephy_dialog_get_control (d, PROP_DETAILS_REMAINING);
	priv->details_progress = ephy_dialog_get_control (d, PROP_DETAILS_PROGRESS);
	priv->details_button = ephy_dialog_get_control (d, PROP_DETAILS_BUTTON);
	priv->open_button = ephy_dialog_get_control (d, PROP_OPEN_BUTTON);
	priv->pause_button = ephy_dialog_get_control (d, PROP_PAUSE_BUTTON);
	priv->abort_button = ephy_dialog_get_control (d, PROP_ABORT_BUTTON);
	details_table = ephy_dialog_get_control (d, PROP_DETAILS_TABLE);

	/* create file and location details labels */
	priv->details_location = ephy_ellipsizing_label_new ("");
	gtk_table_attach_defaults (GTK_TABLE(details_table),
				   priv->details_location,
				   1, 2, 1, 2);
	gtk_misc_set_alignment (GTK_MISC(priv->details_location), 0, 0);
	gtk_label_set_selectable (GTK_LABEL(priv->details_location), TRUE);
	gtk_widget_show (priv->details_location);

	liststore = gtk_list_store_new (5,
					G_TYPE_INT,
					G_TYPE_STRING,
					G_TYPE_STRING,
					G_TYPE_STRING,
					G_TYPE_OBJECT);

	gtk_tree_view_set_model (GTK_TREE_VIEW(priv->treeview),
				 GTK_TREE_MODEL (liststore));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(priv->treeview),
					   TRUE);

	renderer = gtk_cell_renderer_text_new ();

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW(priv->treeview),
						     0, _("%"),
						     renderer,
						     "text", 0,
						     NULL);
	column = gtk_tree_view_get_column (GTK_TREE_VIEW(priv->treeview), 0);
        gtk_tree_view_column_set_resizable (column, TRUE);
        gtk_tree_view_column_set_reorderable (column, TRUE);
        gtk_tree_view_column_set_sort_column_id (column, COL_PERCENT);

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW(priv->treeview),
						     COL_FILENAME, _("Filename"),
						     renderer,
						     "text", COL_FILENAME,
						     NULL);
	column = gtk_tree_view_get_column (GTK_TREE_VIEW(priv->treeview),
					   COL_FILENAME);
        gtk_tree_view_column_set_resizable (column, TRUE);
        gtk_tree_view_column_set_reorderable (column, TRUE);
        gtk_tree_view_column_set_sort_column_id (column, COL_FILENAME);

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW(priv->treeview),
						     COL_SIZE, _("Size"),
						     renderer,
						     "text", COL_SIZE,
						     NULL);
	column = gtk_tree_view_get_column (GTK_TREE_VIEW(priv->treeview),
					   COL_SIZE);
        gtk_tree_view_column_set_resizable (column, TRUE);
        gtk_tree_view_column_set_reorderable (column, TRUE);
        gtk_tree_view_column_set_sort_column_id (column, COL_SIZE);

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW(priv->treeview),
						     COL_REMAINING, _("Remaining"),
						     renderer,
						     "text", COL_REMAINING,
						     NULL);
	column = gtk_tree_view_get_column (GTK_TREE_VIEW(priv->treeview),
					   COL_REMAINING);
        gtk_tree_view_column_set_resizable (column, TRUE);
        gtk_tree_view_column_set_reorderable (column, TRUE);
        gtk_tree_view_column_set_sort_column_id (column, COL_REMAINING);

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(priv->treeview));
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
        g_signal_connect (G_OBJECT (selection), "changed",
                          G_CALLBACK (downloader_treeview_selection_changed_cb), dv);

	priv->model = GTK_TREE_MODEL (liststore);

	icon = gtk_widget_render_icon (GTK_WIDGET (priv->window),
				       EPHY_STOCK_DOWNLOAD,
				       GTK_ICON_SIZE_MENU,
				       NULL);
	gtk_window_set_icon (GTK_WINDOW(priv->window), icon);
}

void
download_dialog_pause_cb (GtkButton *button, DownloaderView *dv)
{
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	DownloadDetails *details;
	GValue val = {0, };
	EphyDownload *download;
	EphyDownloadStatus status;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(dv->priv->treeview));

	if (!gtk_tree_selection_get_selected (selection, &model, &iter)) return;

	gtk_tree_model_get_value (model, &iter, COL_PERSIST_OBJECT, &val);
	download = g_value_get_object (&val);
	g_value_unset (&val);

	details = g_hash_table_lookup (dv->priv->details_hash,
				       download);
	g_return_if_fail (details);

	/* FIXME */
	status = 0;

	if (status == EPHY_DOWNLOAD_STATUS_DOWNLOADING)
	{
		ephy_download_pause (download);
	}
	else if (status == EPHY_DOWNLOAD_STATUS_PAUSED)
	{
		ephy_download_pause (download);
	}
}

void
download_dialog_abort_cb (GtkButton *button, DownloaderView *dv)
{
}

static void
downloader_treeview_selection_changed_cb (GtkTreeSelection *selection,
					  DownloaderView *dv)
{
}

gboolean
download_dialog_delete_cb (GtkWidget *window, GdkEventAny *event,
			   DownloaderView *dv)
{
	return FALSE;
}

void
download_dialog_details_cb (GtkToggleButton *button,
			    DownloaderView *dv)
{
	GtkWidget *details_frame;

	details_frame = ephy_dialog_get_control (EPHY_DIALOG(dv),
						   PROP_DETAILS_FRAME);
	if (gtk_toggle_button_get_active (button))
	{
		gtk_widget_show (GTK_WIDGET (details_frame));
		dv->priv->show_details = TRUE;
	}
	else
	{
		gtk_widget_hide (GTK_WIDGET (details_frame));
		dv->priv->show_details = FALSE;
	}

}

static void
open_selection_foreach (GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter,
			DownloaderView *dv)
{
}

void
download_dialog_open_cb (GtkWidget *button,
			 DownloaderView *dv)
{
	GtkTreeSelection *selection;

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(dv->priv->treeview));

	gtk_tree_selection_selected_foreach
		(selection,
		 (GtkTreeSelectionForeachFunc)open_selection_foreach,
		 (gpointer)dv);
}
