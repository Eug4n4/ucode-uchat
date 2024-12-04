#include "client.h"

t_gtk_sign_in     *gtk_sign_in          = NULL;
t_gtk_sign_up     *gtk_sign_up          = NULL;
t_gtk_main_window *gtk_main_window      = NULL;
t_gtk_create_chat *gtk_create_chat      = NULL;
GtkBuilder        *builder_main_window  = NULL;
GtkBuilder        *builder_create_chat  = NULL;
GtkBuilder        *builder_login        = NULL;
GtkBuilder        *builder_registration = NULL;

void on_btn_sign_in_clicked(GtkButton *button, gpointer data) {
    const gchar *username = gtk_entry_get_text(gtk_sign_in->entry_username);
    const gchar *password = gtk_entry_get_text(gtk_sign_in->entry_password);

    if (strlen(username) == 0 || strlen(password) == 0) {
        gtk_label_set_text(gtk_sign_in->label_error, "Fields cannot be empty");
        return;
    }

    if (!check_username(username)) {
        gtk_label_set_text(gtk_sign_in->label_error, "Invalid username format.");
        return;
    }

    if (send_login_request(username, password, client_data->ssl) < 0) {
        gtk_label_set_text(gtk_sign_in->label_error, "Error communicating with server");
        return;
    }
    g_mutex_lock(&client_data->data_mutex);
    free_user_data(client_data->current_user);
    client_data->current_user->username = g_strdup(username);
    client_data->current_user->password = g_strdup(password);
    g_mutex_unlock(&client_data->data_mutex);

    gtk_entry_set_text(gtk_sign_in->entry_username, "");
    gtk_entry_set_text(gtk_sign_in->entry_password, "");
    gtk_label_set_text(gtk_sign_in->label_error, "");

    (void)button;
    (void)data;
}

void on_btn_sign_up_clicked(GtkButton *button, gpointer data) {
    const gchar *username = gtk_entry_get_text(gtk_sign_up->entry_username);
    const gchar *password = gtk_entry_get_text(gtk_sign_up->entry_password);

    if (strlen(username) == 0 || strlen(password) == 0) {
        gtk_label_set_text(gtk_sign_up->label_error, "Fields cannot be empty");
    } else if (strcmp(username, password) == 0) {
        gtk_label_set_text(gtk_sign_up->label_error, "Username and password cannot be the same.");
    } else {
        if (!check_username(username)) {
            gtk_label_set_text(gtk_sign_up->label_error,
                               "Username requirements:\n"
                               "- Must be longer than 5 characters.\n"
                               "- Only Latin letters, numbers, and underscores are allowed.");
        } else if (!check_password(password)) {
            gtk_label_set_text(gtk_sign_up->label_error,
                               "Password requirements:\n"
                               "- Must be at least 8 characters long.\n"
                               "- Must contain at least:\n"
                               "  - One uppercase letter\n"
                               "  - One lowercase letter\n"
                               "  - One digit\n"
                               "  - One special character.");
        } else {
            if (send_registration_request(username, password, client_data->ssl) < 0) {
                gtk_label_set_text(gtk_sign_up->label_error, "Error communicating with server");
                return;
            }
            g_mutex_lock(&client_data->data_mutex);
            free_user_data(client_data->current_user);
            client_data->current_user->username = g_strdup(username);
            client_data->current_user->password = g_strdup(password);
            g_mutex_unlock(&client_data->data_mutex);

            gtk_entry_set_text(gtk_sign_up->entry_username, "");
            gtk_entry_set_text(gtk_sign_up->entry_password, "");
            gtk_label_set_text(gtk_sign_up->label_error, "");
        }
    }
    (void)button;
    (void)data;
    g_print("Sign up button clicked\n");
}

void on_btn_sign_up_small_clicked(GtkButton *button, gpointer data) {
    if (gtk_sign_up && gtk_sign_up->window && gtk_sign_in && gtk_sign_in->window) {
        gtk_entry_set_text(gtk_sign_in->entry_username, "");
        gtk_entry_set_text(gtk_sign_in->entry_password, "");
        gtk_label_set_text(gtk_sign_in->label_error, "");
        show_screen(REGISTRATION_SCREEN);
    } else {
        g_print("Error in on_btn_sign_up_small_clicked\n");
    }
    (void)button;
    (void)data;
}

void on_btn_sign_in_small_clicked(GtkButton *button, gpointer data) {
    if (gtk_sign_up && gtk_sign_up->window && gtk_sign_in && gtk_sign_in->window) {
        gtk_entry_set_text(gtk_sign_up->entry_username, "");
        gtk_entry_set_text(gtk_sign_up->entry_password, "");
        gtk_label_set_text(gtk_sign_up->label_error, "");
        show_screen(LOGIN_SCREEN);
    } else {
        g_print("Error in on_btn_sign_in_small_clicked\n");
    }
    (void)button;
    (void)data;
}

void on_toggle_renderer_toggled(GtkCellRendererToggle *cell, gchar *path) {
    g_print("toggle renderer toggled\n");
    gboolean      state;
    GtkTreeModel *model = gtk_tree_view_get_model(gtk_create_chat->view_users);
    GtkTreeIter   iter;
    gtk_tree_model_get_iter_from_string(model, &iter, path);
    gtk_tree_model_get(model, &iter, 1, &state, -1);
    state = !state;
    gtk_list_store_set(gtk_create_chat->users_store, &iter, 1, state, -1);
    (void)cell;
}

void on_selected_user_changed(GtkTreeSelection *selection) {
    g_print("Changed\n");
    (void)selection;
}

void on_btn_create_chat_main_clicked(GtkWidget *button, gpointer data) {
    printf("add chat button clicked\n");
    send_all_users_exclude_request(client_data);
    show_screen(CREATE_CHAT_SCREEN);
    gtk_list_store_clear(gtk_create_chat->users_store);
    (void)button;
    (void)data;
}

void on_log_out_subbtn_activate(GtkWidget *log_out_subbtn, gpointer data) {
    g_print("Log out button clicked\n");
    g_mutex_lock(&client_data->data_mutex);
    gtk_entry_set_text(gtk_main_window->entry_send_message, "");
    client_data->is_logged_in = false;
    g_mutex_unlock(&client_data->data_mutex);
    show_screen(LOGIN_SCREEN);
    (void)log_out_subbtn;
    (void)data;
}

void on_btn_create_chat_clicked(GtkButton *button, gpointer data) {
    t_app *app = (t_app *)data;
    g_print("button create chat clicked\n");
    const gchar *chat_name = gtk_entry_get_text(gtk_create_chat->entry_chat_name);
    gboolean     state;
    gchar       *username;

    if (chat_name[0] == '\0') {
        gtk_label_set_text(gtk_create_chat->label_status, "Enter chat name to create a new chat");
        return;
    }

    GtkTreeModel *model = gtk_tree_view_get_model(gtk_create_chat->view_users);
    GtkTreeIter   iter;
    gint          selected_users_count = 0;
    if (gtk_tree_model_get_iter_from_string(model, &iter, "0")) {
        if (!app->users) {
            app->users = create_users(NULL);
        }
        do {
            gtk_tree_model_get(model, &iter, 0, &username, -1);
            gtk_tree_model_get(model, &iter, 1, &state, -1);
            if (state) {
                t_user *user   = create_user();
                user->username = g_strdup(username);
                add_users_front(&app->users, user);
                selected_users_count++;
            }
            g_free(username);
        } while (gtk_tree_model_iter_next(model, &iter));
        if (selected_users_count > 0) {
            send_create_chat_request(app, chat_name);
        } else {
            gtk_label_set_text(gtk_create_chat->label_status, "Select at least one user");
        }
    }
    gtk_entry_set_text(gtk_create_chat->entry_chat_name, "");
    (void)button;
    (void)data;
}

void on_btn_send_message_clicked(GtkButton *button, gpointer data) {
    gint chat_id;
    GtkTreeIter   iter;
    GtkTreeModel *model = gtk_tree_view_get_model(gtk_main_window->chats_list_view);
    
    if (gtk_tree_selection_get_selected(gtk_main_window->chat_selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 3, &chat_id, -1);
        printf("%d\n",chat_id);
    }
    (void)button;
    (void)data;

}

void on_chat_selection_changed(GtkTreeSelection *selection) {
    printf("changed\n");
    gchar        *chat_name;
    gint          chat_members;
    gint          chat_id;
    GtkTreeIter   iter;
    GtkTreeModel *model = gtk_tree_view_get_model(gtk_main_window->chats_list_view);

    if (!gtk_tree_model_get_iter_first(model, &iter)) {
        printf("Empty\n");
        gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_chat_name));
        gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_members_count));
        gtk_widget_hide(GTK_WIDGET(gtk_main_window->entry_send_message));
        gtk_widget_hide(GTK_WIDGET(gtk_main_window->btn_send_message));
        return;
    }

    if (!gtk_widget_is_visible(GTK_WIDGET(gtk_main_window->label_chat_name))) {
        gtk_widget_show(GTK_WIDGET(gtk_main_window->label_chat_name));
    }
    if (!gtk_widget_is_visible(GTK_WIDGET(gtk_main_window->label_members_count))) {
        gtk_widget_show(GTK_WIDGET(gtk_main_window->label_members_count));
    }
    if (!gtk_widget_is_visible(GTK_WIDGET(gtk_main_window->entry_send_message))) {
        gtk_widget_show(GTK_WIDGET(gtk_main_window->entry_send_message));
    }
    if (!gtk_widget_is_visible(GTK_WIDGET(gtk_main_window->btn_send_message))) {
        gtk_widget_show(GTK_WIDGET(gtk_main_window->btn_send_message));
    }

    if (client_data->is_logged_in && gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 1, &chat_name, -1);
        gtk_tree_model_get(model, &iter, 2, &chat_members, -1);
        gtk_tree_model_get(model, &iter, 3, &chat_id, -1);

        send_get_chat_messages_request(chat_id);
        
        gchar *str_members_count = g_strdup_printf("Members: %d", chat_members);
        gtk_label_set_text(gtk_main_window->label_chat_name, chat_name);
        gtk_label_set_text(gtk_main_window->label_members_count, str_members_count);
        g_free(str_members_count);
        g_free(chat_name);
    }
}

gboolean close_reconnect_popup(GtkWidget *dialog) {
    if (GTK_IS_WIDGET(dialog)) {
        gtk_widget_destroy(dialog);
        return FALSE;
    }
    return FALSE;
}

void show_reconnect_popup_callback(GtkWidget *dialog) {
    if (GTK_IS_WIDGET(dialog)) {
        gtk_widget_show_all(dialog);
    }
}

GtkWidget *show_reconnect_popup(void) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_sign_in->window),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_NONE,
                                               "Reconnecting to the server... Please wait.");
    if (!GTK_IS_WIDGET(dialog)) {
        g_print("Error: Failed to create dialog widget\n");
        return NULL;
    }

    gtk_window_set_title(GTK_WINDOW(dialog), "Reconnection in Progress");

    GtkWidget *close_button = gtk_button_new_with_label("Close 4hat");
    g_signal_connect(close_button, "clicked", G_CALLBACK(destroy_screens), NULL);  // Terminate the application

    GtkWidget *content_area = gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog));
    gtk_box_pack_end(GTK_BOX(content_area), close_button, FALSE, FALSE, 0);

    gtk_widget_show_all(dialog);

    return dialog;
}

void init_gui(int argc, char **argv, t_app *app) {
    gtk_init(&argc, &argv);

    gtk_sign_in = create_gtk_sign_in_data();
    if (!gtk_sign_in) {
        printf("Error initializing GUI\n");
        exit(EXIT_FAILURE);
    }

    gtk_sign_up     = create_gtk_sign_up_data();
    gtk_main_window = create_gtk_main_window_data();
    gtk_create_chat = create_gtk_create_chat_data();

    GtkButton *btn_sign_in       = GTK_BUTTON(gtk_builder_get_object(builder_login, "btn_sign_in"));
    GtkButton *btn_sign_up_small = GTK_BUTTON(gtk_builder_get_object(builder_login, "btn_sign_up_small"));
    g_signal_connect(btn_sign_in, "clicked", G_CALLBACK(on_btn_sign_in_clicked), NULL);
    g_signal_connect(btn_sign_up_small, "clicked", G_CALLBACK(on_btn_sign_up_small_clicked), NULL);

    GtkButton *btn_sign_up       = GTK_BUTTON(gtk_builder_get_object(builder_registration, "btn_sign_up"));
    GtkButton *btn_sign_in_small = GTK_BUTTON(gtk_builder_get_object(builder_registration, "btn_sign_in_small"));
    g_signal_connect(btn_sign_up, "clicked", G_CALLBACK(on_btn_sign_up_clicked), NULL);
    g_signal_connect(btn_sign_in_small, "clicked", G_CALLBACK(on_btn_sign_in_small_clicked), NULL);

    GtkWidget *log_out_btn          = GTK_WIDGET(gtk_builder_get_object(builder_main_window, "log_out_subbtn"));
    GtkButton *btn_create_chat_main = GTK_BUTTON(gtk_builder_get_object(builder_main_window, "btn_create_chat_main"));
    g_signal_connect(gtk_main_window->chat_selection, "changed", G_CALLBACK(on_chat_selection_changed), NULL);
    g_signal_connect(log_out_btn, "activate", G_CALLBACK(on_log_out_subbtn_activate), NULL);
    g_signal_connect(btn_create_chat_main, "clicked", G_CALLBACK(on_btn_create_chat_main_clicked), NULL);

    GtkButton *btn_create_chat = GTK_BUTTON(gtk_builder_get_object(builder_create_chat, "btn_create_chat"));
    g_signal_connect(gtk_create_chat->toggle_renderer, "toggled", G_CALLBACK(on_toggle_renderer_toggled), NULL);
    g_signal_connect(gtk_create_chat->selected_user, "changed", G_CALLBACK(on_selected_user_changed), NULL);
    g_signal_connect(btn_create_chat, "clicked", G_CALLBACK(on_btn_create_chat_clicked), app);

    g_signal_connect(gtk_sign_in->window, "destroy", G_CALLBACK(destroy_screens), NULL);
    g_signal_connect(gtk_sign_up->window, "destroy", G_CALLBACK(destroy_screens), NULL);
    g_signal_connect(gtk_main_window->window, "destroy", G_CALLBACK(destroy_screens), NULL);
    g_signal_connect(gtk_create_chat->window, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
    g_signal_connect(gtk_main_window->btn_send_message, "clicked", G_CALLBACK(on_btn_send_message_clicked), NULL);
    
    show_screen(LOGIN_SCREEN);
}
