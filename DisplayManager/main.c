#include <gtk/gtk.h>
struct Data {
	GtkWidget *entry_name;
	GtkWidget *entry_password;
};
static void button_clicked(GtkWidget *widget,gpointer _data){
	struct Data *data = (struct Data*) _data;
	g_print("%s\n", gtk_editable_get_text(GTK_EDITABLE(data->entry_name)));
	g_print("%s\n", gtk_editable_get_text(GTK_EDITABLE(data->entry_password)));
}

static void activate(GApplication *app, gpointer *user_data) {
	GtkWidget *entry_password, *label_password, *box_password;
	entry_password = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(entry_password), false);
	label_password = gtk_label_new("Password:");
	box_password = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_append(GTK_BOX(box_password), label_password);
	gtk_box_append(GTK_BOX(box_password), entry_password);
	
	GtkWidget *entry_name, *label_name, *box_name;
	entry_name = gtk_entry_new();
	gtk_widget_set_size_request(entry_name, 300, -1);
	label_name = gtk_label_new("User: ");
	box_name = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_append(GTK_BOX(box_name), label_name);
	gtk_box_append(GTK_BOX(box_name), entry_name);
		
	GtkWidget *box;
	GtkWidget *button;
	button = gtk_button_new_with_label("Login");
	gtk_widget_set_size_request(button, 100, -1);
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
	static struct Data data;
	data.entry_name = entry_name;
	data.entry_password = entry_password;
	g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), &data);
	gtk_box_append(GTK_BOX(box), box_name);
	gtk_box_append(GTK_BOX(box), box_password);
	gtk_box_append(GTK_BOX(box), button);
	gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

	GtkWidget *win;
	win = gtk_window_new();
	gtk_window_fullscreen(GTK_WINDOW(win));
	gtk_window_set_title(GTK_WINDOW(win), "GingerOS");
	gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));
	gtk_window_set_child(GTK_WINDOW(win), box);
	gtk_widget_show(win);
}
int
main (int    argc,
      char **argv)
{ 
  GtkApplication *app;
  int status;

  GtkCssProvider *css_provider;
  css_provider = gtk_css_provider_new();
  GFile *css_file;
  css_file = g_file_new_for_path("main.css");
  gtk_css_provider_load_from_file(css_provider, css_file);

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
   return status;
} 
