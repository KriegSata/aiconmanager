#include <gtk/gtk.h>
#include "trial.h"

// Global widgets that need to be accessed by callbacks
typedef struct {
    GtkWidget *username_entry;
    GtkWidget *password_entry;
    GtkWidget *login_label;
    GtkWidget *stack;
    GtkWidget *main_window;
} AppWidgets;

static void activate(GtkApplication *app, gpointer user_data) {
    AppWidgets *widgets = g_malloc(sizeof(AppWidgets));
    
    // Create main window
    widgets->main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(widgets->main_window), "Aircon Service System");
    gtk_window_set_default_size(GTK_WINDOW(widgets->main_window), 800, 600);

    // Create stack for different screens
    widgets->stack = gtk_stack_new();
    gtk_container_add(GTK_CONTAINER(widgets->main_window), widgets->stack);
    
    // Create login page
    GtkWidget *login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(login_box), 20);
    
    // Add title
    GtkWidget *title_label = gtk_label_new("Aircon Service System");
    PangoAttrList *attr_list = pango_attr_list_new();
    PangoAttribute *size_attr = pango_attr_size_new(20 * PANGO_SCALE);
    pango_attr_list_insert(attr_list, size_attr);
    gtk_label_set_attributes(GTK_LABEL(title_label), attr_list);
    pango_attr_list_unref(attr_list);
    
    // Create form grid
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    
    // Username
    GtkWidget *username_label = gtk_label_new("Username:");
    widgets->username_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->username_entry, 1, 0, 1, 1);
    
    // Password
    GtkWidget *password_label = gtk_label_new("Password:");
    widgets->password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(widgets->password_entry), FALSE);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->password_entry, 1, 1, 1, 1);
    
    // Login button
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), widgets);
    
    // Register button
    GtkWidget *register_button = gtk_button_new_with_label("Register");
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_clicked), widgets);
    
    // Add widgets to login box
    gtk_box_pack_start(GTK_BOX(login_box), title_label, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(login_box), grid, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(login_box), login_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(login_box), register_button, FALSE, FALSE, 10);
    
    // Add login page to stack
    gtk_stack_add_named(GTK_STACK(widgets->stack), login_box, "login");
    
    // Show all widgets
    gtk_widget_show_all(widgets->main_window);
}

int main(int argc, char **argv) {
    // Initialize your existing data
    loadAdminData();
    loadUserData();
    loadAirconData();
    loadTechnicianData();
    loadServiceRequestData();
    loadAirconAssignments();

    // Create GTK application
    GtkApplication *app = gtk_application_new("org.aircon.service", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
