#include <gtk/gtk.h>

void on_button_clicked(GtkWidget *widget, gpointer data) {
    system((char *)data);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *button4;
    GtkWidget *button5;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Wiibench");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    vbox = gtk_vbox_new(FALSE, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label = gtk_label_new("<span font='40'>Wiibench</span>");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    button1 = gtk_button_new_with_label("Run Executable 1");
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), "./executable1");
    gtk_box_pack_start(GTK_BOX(vbox), button1, TRUE, TRUE, 0);

    button2 = gtk_button_new_with_label("Run Executable 2");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button_clicked), "./executable2");
    gtk_box_pack_start(GTK_BOX(vbox), button2, TRUE, TRUE, 0);

    button3 = gtk_button_new_with_label("Run Executable 3");
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button_clicked), "./executable3");
    gtk_box_pack_start(GTK_BOX(vbox), button3, TRUE, TRUE, 0);

    button4 = gtk_button_new_with_label("Run Executable 4");
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button_clicked), "./executable4");
    gtk_box_pack_start(GTK_BOX(vbox), button4, TRUE, TRUE, 0);

    button5 = gtk_button_new_with_label("Run Executable 5");
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button_clicked), "./executable5");
    gtk_box_pack_start(GTK_BOX(vbox), button5, TRUE, TRUE, 0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

