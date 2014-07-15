#ifndef LABEL_TOGGLE_ATTR_H
#define LABEL_TOGGLE_ATTR_H

#include <gtk/gtk.h>

enum GtkLabelState
{
    GTK_LABEL_NORMAL_OFF,
    GTK_LABEL_NORMAL_ON
};

typedef struct _GtkPassedData
{
    gint passed_state;
    GtkWidget *passed_label;
}GtkPassedData;

PangoAttrList *
label_prelight ();

gint
label_normal_on (GtkLabel *label);

gint
label_normal_off (GtkLabel *label);

#endif
