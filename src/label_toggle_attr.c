#include "label_toggle_attr.h"

void
label_underline (GtkWidget *label)
{
    PangoAttrList *list;
    PangoAttribute *attr;

    list = pango_attr_list_new ();
    attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);

    pango_attr_list_insert (list, attr);

    gtk_label_set_attributes (GTK_LABEL (label), list);
}

PangoAttrList *
label_prelight ()
{
    PangoAttrList *list;
    PangoAttribute *attr;

    list = pango_attr_list_new ();
    //attr = pango_attr_foreground_new (65535, 65535, 65535);
    //pango_attr_list_insert (list, attr);
    //attr = pango_attr_background_new (13000, 13000, 13000);
    //pango_attr_list_insert (list, attr);
    attr = pango_attr_weight_new (PANGO_WEIGHT_ULTRALIGHT);
    pango_attr_list_insert (list, attr);
    //attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
    //pango_attr_list_insert (list, attr);

    return list;
}

gint
label_normal_on (GtkLabel *label)
{
    PangoAttrList *list;
    PangoAttribute *attr;

    list = pango_attr_list_new ();
    //attr = pango_attr_foreground_new (58535, 58535, 58535);
    //pango_attr_list_insert (list, attr);
    attr = pango_attr_weight_new (PANGO_WEIGHT_SEMIBOLD);
    pango_attr_list_insert (list, attr);

    gtk_label_set_attributes (GTK_LABEL (label), list);

    return GTK_LABEL_NORMAL_ON;
}

gint
label_normal_off (GtkLabel *label)
{
    PangoAttrList *list;
    PangoAttribute *attr;

    list = pango_attr_list_new ();
    attr = pango_attr_foreground_new (27323, 27323, 27323);
    pango_attr_list_insert (list, attr);
    attr = pango_attr_weight_new (PANGO_WEIGHT_NORMAL);
    pango_attr_list_insert (list, attr);

    gtk_label_set_attributes (GTK_LABEL (label), list);

    return GTK_LABEL_NORMAL_OFF;
}
