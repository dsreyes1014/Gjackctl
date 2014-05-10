#include "rt_priority.h"

void
value_cb (GtkSpinButton *spin_button, gpointer data)
{
	gint priority_num;
	const gchar *priority_arg;
	gchar number[10];

	priority_num = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spin_button));
	g_sprintf (number, "%d", priority_num);
	priority_arg = "-P";

	jack_start[2] = g_strconcat (priority_arg, number, NULL);
}

void
rt_priority (GtkWidget *grid)
{
	GtkWidget *label;
	GtkWidget *spin_button;
	GtkAdjustment *adjustment;
	gint priority_num;
	const gchar *priority_arg;
	gchar number[10];

	label = gtk_label_new ("Priority");
	adjustment = gtk_adjustment_new (75, 0, 99, 1, 0, 0);	
	spin_button = gtk_spin_button_new (adjustment, 1, 0);
	
	/* Align widgets within `grid` to keep things uniform. */
	gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
	gtk_widget_set_valign (label, GTK_ALIGN_END);
	gtk_widget_set_halign (spin_button, GTK_ALIGN_START);

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
	gtk_grid_attach (GTK_GRID (grid), label, 2, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), spin_button, label, GTK_POS_BOTTOM, 1, 1);

	priority_num = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spin_button));
	
	/* Convert `gint priority_num` to gchar with `g_sprintf` statement and combine 
	with `priority_arg` using the `g_strconcat` function to create an element in 
	the `jack_start[]` array. We initialize the element using the `g_strconcat`
	function.*/
	g_sprintf (number, "%d", priority_num);
	priority_arg = "-P";
	jack_start[2] = g_strconcat (priority_arg, number, NULL);	

	g_signal_connect (spin_button, "value-changed", G_CALLBACK (value_cb), NULL);
}
