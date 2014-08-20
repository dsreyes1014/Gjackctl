#include "jack_log.h"

void
jack_log (GtkWidget *stack,
          GtkWidget *text)
{
    GtkWidget *box;

    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);

    gtk_box_pack_start (GTK_BOX (box), text, FALSE, FALSE, 2);

    gtk_stack_add_titled (GTK_STACK (stack), box, "log", "Log");
}
