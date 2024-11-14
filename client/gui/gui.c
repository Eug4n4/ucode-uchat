#include "gui_structs.h"
// clang gui.c -o gui `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -Igui_structs.h

t_gtk_sign_up *create_gtk_sign_up_data(t_gtk_sign_in *gtk_sign_in) {
    t_gtk_sign_up *data = malloc(sizeof(t_gtk_sign_up));
    GtkWindow *window;
    GError *error = NULL;
    GtkBuilder *builder = gtk_builder_new();
    GtkEntry *entry_username;
    GtkEntry *entry_password;
    GtkLabel *label_error;

    if (gtk_builder_add_from_file(builder,GLADE_REGISTER_PATH,&error) == 0) {
        g_print("Error loading file %s\n",error->message);
        free(data);
        g_error_free(error);
        return NULL;
    }
    window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));
    entry_username = GTK_ENTRY(gtk_builder_get_object(builder,"entry_username"));
    entry_password = GTK_ENTRY(gtk_builder_get_object(builder,"entry_password"));
    label_error = GTK_LABEL(gtk_builder_get_object(builder,"label_error"));
    data->window = window;
    data->builder = builder;
    data->entry_username = entry_username;
    data->entry_password = entry_password;
    data->label_error = label_error;
    gtk_sign_in->gtk_sign_up = data;
    return data;

}

t_gtk_sign_in *create_gtk_sign_in_data(void) {
    t_gtk_sign_in *data = malloc(sizeof(t_gtk_sign_in));
    GtkBuilder *builder;
    GtkWindow *window;
    GtkEntry *entry_username;
    GtkEntry *entry_password;
    GtkLabel *label_error;
    GError *error = NULL;

    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, GLADE_LOGIN_PATH, &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        free(data);
        g_clear_error(&error);
        return NULL;
    }


    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    if (!window) {
        g_printerr("Error creating window: %s\n", error->message);
        free(data);
        g_clear_error(&error);
        return NULL;
    }

    entry_username = GTK_ENTRY(gtk_builder_get_object(builder, "entry_username"));
    entry_password = GTK_ENTRY(gtk_builder_get_object(builder, "entry_password"));
    label_error = GTK_LABEL(gtk_builder_get_object(builder, "label_error"));

    data->window = window;
    data->entry_username = entry_username;
    data->entry_password = entry_password;
    data->label_error = label_error;
    data->builder = builder;
    data->gtk_sign_up = NULL;

    return data;
}


void on_sign_in_clicked(GtkButton *button, gpointer data) {
    if (data) {
        const gchar *username = gtk_entry_get_text(((t_gtk_sign_in *)data)->entry_username);
        const gchar *password = gtk_entry_get_text(((t_gtk_sign_in *)data)->entry_password);

        if (strlen(password) == 0 || strlen(username) == 0) {
            gtk_label_set_text(((t_gtk_sign_in *)data)->label_error, "Fields can not be empty");
        } else {
            gtk_label_set_text(((t_gtk_sign_in *)data)->label_error, "");
            g_print("Data\n");
            g_print("%s\n", username);
            g_print("%s\n", password);
            gtk_entry_set_text(((t_gtk_sign_in *)data)->entry_username, "");
            gtk_entry_set_text(((t_gtk_sign_in *)data)->entry_password, "");
        }
    }
    g_print("Sign in button clicked\n");
}


void on_sign_up_btn_clicked(GtkButton *button, gpointer data) {
    if (data) {
        const gchar *username = gtk_entry_get_text(((t_gtk_sign_up *)data)->entry_username);
        const gchar *password = gtk_entry_get_text(((t_gtk_sign_up *)data)->entry_password);

        if (strlen(password) == 0 || strlen(username) == 0) {
            gtk_label_set_text(((t_gtk_sign_up *)data)->label_error, "Fields can not be empty");
        } else {
            gtk_label_set_text(((t_gtk_sign_up *)data)->label_error, "");
            g_print("Data\n");
            g_print("%s\n", username);
            g_print("%s\n", password);
            gtk_entry_set_text(((t_gtk_sign_up *)data)->entry_username, "");
            gtk_entry_set_text(((t_gtk_sign_up *)data)->entry_password, "");
        }
    }
    g_print("Sign up button clicked\n");
}


void on_sign_in_btn_clicked(GtkButton *button, gpointer data) {
    if (data) {
        gtk_widget_hide(GTK_WIDGET(((t_gtk_sign_in *)data)->gtk_sign_up->window));
        gtk_entry_set_text(((t_gtk_sign_in *)data)->gtk_sign_up->entry_username, "");
        gtk_entry_set_text(((t_gtk_sign_in *)data)->gtk_sign_up->entry_username, "");
        gtk_label_set_text(((t_gtk_sign_in *)data)->label_error, "");

        gtk_widget_show_all(GTK_WIDGET(((t_gtk_sign_in *)data)->window));
    }
}


void sign_up_connect_signals(t_gtk_sign_in *gtk_sign_in, t_gtk_sign_up *gtk_sign_up) {
    GtkButton *btn_sign_in;
    GtkButton *btn_sign_up;
    g_signal_connect(gtk_sign_up->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    btn_sign_in = GTK_BUTTON(gtk_builder_get_object(gtk_sign_up->builder, "btn_sign_in"));
    btn_sign_up = GTK_BUTTON(gtk_builder_get_object(gtk_sign_up->builder, "btn_sign_up"));
    g_signal_connect(btn_sign_in, "clicked", G_CALLBACK(on_sign_in_btn_clicked), gtk_sign_in);
    g_signal_connect(btn_sign_up, "clicked", G_CALLBACK(on_sign_up_btn_clicked), gtk_sign_up);

}


void on_sign_up_clicked(GtkButton *button, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(((t_gtk_sign_in *)data)->window));
    gtk_entry_set_text(((t_gtk_sign_in *)data)->entry_username, "");
    gtk_entry_set_text(((t_gtk_sign_in *)data)->entry_password, "");

    t_gtk_sign_up *gtk_sign_up = create_gtk_sign_up_data((t_gtk_sign_in *)data);
    if (!gtk_sign_up) {
        gtk_main_quit();
    }
    sign_up_connect_signals((t_gtk_sign_in *)data, gtk_sign_up);

    g_print("Sign up button clicked\n");
}

void sign_in_connect_signals(t_gtk_sign_in *gtk_sign_in) {
    GtkButton *btn_sign_in;
    GtkButton *btn_sign_up;
    g_signal_connect(gtk_sign_in->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    btn_sign_in = GTK_BUTTON(gtk_builder_get_object(gtk_sign_in->builder, "btn_sign_in"));
    btn_sign_up = GTK_BUTTON(gtk_builder_get_object(gtk_sign_in->builder, "btn_sign_up"));
    g_signal_connect(btn_sign_in, "clicked", G_CALLBACK(on_sign_in_clicked), gtk_sign_in);
    g_signal_connect(btn_sign_up, "clicked", G_CALLBACK(on_sign_up_clicked), gtk_sign_in);

}


void init_gui(int argc,char **argv) {
    gtk_init(&argc, &argv);
    t_gtk_sign_in *gtk_sign_in = create_gtk_sign_in_data();
    if (!gtk_sign_in) {
        return;
    }

    sign_in_connect_signals(gtk_sign_in);
    gtk_main();
    if (gtk_sign_in->gtk_sign_up) {
        g_object_unref(gtk_sign_in->gtk_sign_up->builder);
        free(gtk_sign_in->gtk_sign_up);
    }
    g_object_unref(gtk_sign_in->builder);
    free(gtk_sign_in);

}



int main(int argc, char **argv) {
    init_gui(argc, argv);
}
