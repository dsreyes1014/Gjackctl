#include "connections.h"

GPid pid_patchage;

void
connections_cb (GtkButton *button, gpointer user_data)
{
	gchar *patchage[2];

	patchage[0] = "patchage";
	patchage[1] = NULL;

	g_spawn_async (NULL, patchage, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &pid_patchage, NULL); 
}

void
connections (GtkWidget *widget)
{
	GtkWidget *conn_button;

	conn_button = gtk_button_new_with_label ("Connect");

	gtk_box_pack_start (GTK_BOX (widget), conn_button, FALSE, TRUE, 2);

	g_signal_connect (conn_button, "clicked", G_CALLBACK (connections_cb), NULL);
}
