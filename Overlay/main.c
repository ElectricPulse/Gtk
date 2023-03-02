#include <gtk/gtk.h>

static void activate(GApplication *app, gpointer *user_data) {
	GtkWidget *win;
	win = gtk_window_new();
	gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));

	GtkWidget *box;
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_set_homogeneous(GTK_BOX(box), TRUE);
	
	GtkWidget *video, *button, *label;
	label = gtk_label_new("This is very mature humor");
	button = gtk_button_new_with_label("Exit");


	video = gtk_video_new_for_filename("/home/hackerman/GTK/AhojSvet/rick.mp4");	
	gtk_video_set_autoplay(GTK_VIDEO(video), 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), win);

	gtk_box_append(GTK_BOX(box), label);
	gtk_box_append(GTK_BOX(box), video);
	gtk_box_append(GTK_BOX(box), button);

	gtk_window_set_child(GTK_WINDOW(win), box);
	
	gtk_widget_show(win);
}
int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
} 
