#include "label_toggle_attr.h"

PangoAttrList *
label_prelight ()
{
    PangoAttrList *list;
    PangoAttribute *attr;

    list = pango_attr_list_new ();
    attr = pango_attr_style_new (PANGO_STYLE_ITALIC);
    pango_attr_list_insert (list, attr);
    attr = pango_attr_foreground_new (65535, 65535, 65535);
    pango_attr_list_insert (list, attr);
    attr = pango_attr_weight_new (PANGO_WEIGHT_ULTRABOLD);
    pango_attr_list_insert (list, attr);

    return list;
}

gint
label_normal_on (GtkLabel *label)
{
    PangoAttrList *list;
    PangoAttribute *attr;

    list = pango_attr_list_new ();
    attr = pango_attr_foreground_new (65535, 65535, 65535);
    pango_attr_list_insert (list, attr);
    attr = pango_attr_weight_new (PANGO_WEIGHT_NORMAL);
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
    attr = pango_attr_foreground_new (32323, 32323, 32323);
    pango_attr_list_insert (list, attr);
    attr = pango_attr_weight_new (PANGO_WEIGHT_NORMAL);
    pango_attr_list_insert (list, attr);

    gtk_label_set_attributes (GTK_LABEL (label), list);

    return GTK_LABEL_NORMAL_OFF;
}
