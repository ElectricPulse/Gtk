#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdbool.h>
#include <stdlib.h>

static bool findImages(char *** const _files, int * const _files_count){
	struct dirent *current_dir;
	DIR *current_path;
	current_path = opendir("./");


	int files_count = *_files_count;
	files_count = 0;
	
	*_files = malloc(sizeof(char*));
	char **files = *_files;

	while((current_dir = readdir(current_path)) != NULL){	
		if(!strcmp(current_dir->d_name,".") || !strcmp(current_dir->d_name, ".."))
			continue;
		
		int i, match_i;
		bool matches = false;
		char extensions[3][5];
		strcpy(extensions[0], ".jpg");
		strcpy(extensions[1], ".jpg");
		extensions[2][0] = '\0';

		for(int x = 0; extensions[x][0]!= '\0'; ++x) {
			char *extension = extensions[x];
			for(i = strlen(current_dir->d_name)-1, match_i=4-1; i >= 0;--i, --match_i){
				if(current_dir->d_name[i] != extension[match_i]) {
					break;
				} 

				if(extension[match_i] == '.') {
					matches = true;
					break;
				}
			}

			if(matches)
				break;
		}

		if(!matches)
			continue;

		char* file_name = malloc(sizeof(char) * (strlen(current_dir->d_name)+1));
		strcpy(file_name,current_dir->d_name);
		files[files_count] = file_name;
		++files_count;
		*_files = realloc(*_files, sizeof(char *) * (files_count+1));
		files = *_files;
	}

	files[files_count] = NULL;

	return 0;
}

struct Data {
	char **files;
	int image_index;
	bool ignoreChange;
	GtkEntryBuffer* buffer;
	GtkWidget* image;
};

static void loadImagePrevious(GtkWidget *button, gpointer _data){
	struct Data *data = _data;
	
	if(data->image_index == 0)
		return;

	--data->image_index;
	
	gtk_image_set_from_file(GTK_IMAGE(data->image), data->files[data->image_index]);
	gtk_entry_buffer_set_text(data->buffer, data->files[data->image_index], strlen(data->files[data->image_index]));
	data->ignoreChange = true;
}


static void loadImageNext(GtkWidget *button, gpointer _data){
	struct Data *data = _data;
	
	if(data->files[data->image_index+1] == NULL)
		return;

	++data->image_index;


	gtk_image_set_from_file(GTK_IMAGE(data->image), data->files[data->image_index]);
	gtk_entry_buffer_set_text(data->buffer, data->files[data->image_index], strlen(data->files[data->image_index]));
	data->ignoreChange = true;
}

static void deletePicture(GtkWidget* self, struct Data *data){
	remove(data->files[data->image_index]);
}
static void changeName(GtkWidget* self, struct Data *data){
	if(data->ignoreChange) {
		data->ignoreChange = false;
		return;
	}

	const char* text = gtk_entry_buffer_get_text(data->buffer);
	
	printf("%s\n", text);	
	rename(data->files[data->image_index],text);
	strcpy(data->files[data->image_index], text);
}

static void activate(GApplication *app, gpointer *user_data) {
	static char **files;
	bool result;
	int images_count;
	result = findImages(&files, &images_count);
	
	char *file_name;
	//while((file_name = *files++) != NULL)
	//	g_print("%s\n", file_name);

	if(result)
		printf("Couldnt access files\n");
		
	GtkWidget *box;
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL,15);

	GtkWidget *image;
	image = gtk_image_new();
	
	gtk_image_set_pixel_size(GTK_IMAGE(image), 500);

	GtkWidget *filename_entry;

	GtkEntryBuffer *buffer;
	buffer = gtk_entry_buffer_new(NULL, 0);
	filename_entry = gtk_entry_new_with_buffer(buffer);

		GtkWidget *button_box;
	button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,25);
	GtkWidget *button_previous, *button_next;
	button_previous = gtk_button_new_with_label("<-- Previous");

	button_next = gtk_button_new_with_label("Next -->");
	
	static struct Data data;

	data.files = files;
	data.image = image;
	data.buffer = buffer;
	data.image_index = -1;
	
	g_signal_connect(button_next, "clicked", G_CALLBACK(loadImageNext), &data);
	g_signal_connect(button_previous, "clicked", G_CALLBACK(loadImagePrevious), &data);
	g_signal_connect(GTK_EDITABLE(filename_entry), "changed", G_CALLBACK(changeName), &data);


	loadImageNext(button_next, &data);

	gtk_box_append(GTK_BOX(button_box), button_previous);
	gtk_box_append(GTK_BOX(button_box), button_next);
	
	gtk_widget_set_halign(button_box, GTK_ALIGN_CENTER);
	gtk_box_append(GTK_BOX(box), image);
	gtk_box_append(GTK_BOX(box), button_box);
	gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
	gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

	gtk_box_append(GTK_BOX(box), filename_entry);
	GtkWidget* button_delete;
	button_delete = gtk_button_new_with_label("Delete");
	g_signal_connect(button_delete, "clicked", G_CALLBACK(deletePicture), &data);
	gtk_box_append(GTK_BOX(box), button_delete);

	GtkWidget *win;
	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), "Gallery");
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
