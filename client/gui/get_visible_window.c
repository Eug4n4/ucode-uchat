#include "gui.h"

GtkWindow* get_visible_window(void) {
    GList *windows = gtk_window_list_toplevels();
    GtkWindow *visible_window = NULL;

    for (GList *iter = windows; iter != NULL; iter = iter->next) {
        GtkWindow *window = GTK_WINDOW(iter->data);

        if (gtk_widget_get_visible(GTK_WIDGET(window))) {
            visible_window = window;
            break;
        }
    }
    
    g_list_free(windows);

    return visible_window;
}

