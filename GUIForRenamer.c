#include<stdio.h>
#include<gtk/gtk.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include"data_fields.h"

#define WAIT_TEXT "Processing file(s). Please wait while the process gets completed in some time."
#define PROCESS_COMPLETED "Process completed successfully!"
#define PROCESS_FAILED "Process failed!"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *srcExtText;
GtkWidget *destExtText;
GtkWidget *srcPathText;
GtkWidget *destPathText;
GtkWidget *srcFileChooser;
GtkWidget *destFileChooser;
GtkWidget *applyBtn;
GtkWidget *cancelBtn;
GtkWidget *statusLabel;
GtkWidget *statusDesc;
GtkTextBuffer *srcPathBuf;
GtkTextBuffer *srcExtBuf;
GtkTextBuffer *destPathBuf;
GtkTextBuffer *destExtBuf;
char *SrcPath, *DestPath, *SrcExt, *DestExt;
char *ErrorDesc, *StatusDesc;
char * getTextFromTextWidget(GtkWidget *Text, GtkTextBuffer *TextBuf);
gboolean key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data);
void Set_Status(int state);
void Process_Rename();

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "GUI for Extension Renamer.glade", NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
	srcExtText = GTK_WIDGET(gtk_builder_get_object(builder, "SourceExtension"));
	destExtText = GTK_WIDGET(gtk_builder_get_object(builder, "DestinationExtension"));
	srcPathText = GTK_WIDGET(gtk_builder_get_object(builder, "SourcePath"));
	srcFileChooser = GTK_WIDGET(gtk_builder_get_object(builder, "SourceFileChooser"));
	destPathText = GTK_WIDGET(gtk_builder_get_object(builder, "DestinationPath"));
	destFileChooser = GTK_WIDGET(gtk_builder_get_object(builder, "DestinationFileChooser"));
	applyBtn = GTK_WIDGET(gtk_builder_get_object(builder, "Rename"));
	cancelBtn = GTK_WIDGET(gtk_builder_get_object(builder, "Cancel"));
	statusLabel = GTK_WIDGET(gtk_builder_get_object(builder, "StatusLabel"));
	statusDesc = GTK_WIDGET(gtk_builder_get_object(builder, "StatusDesc"));
	srcPathBuf = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "SrcPathBuffer"));
	destPathBuf = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "DestPathBuffer"));
	gtk_builder_connect_signals(builder, NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(window, "key_press_event", G_CALLBACK(key_press_event), NULL);
	gtk_widget_show(window);
	gtk_main();
	return EXIT_SUCCESS;
}

void on_MainWindow_destroy()
{
	gtk_main_quit();
}

void on_Cancel_clicked (GtkButton *b) 
{
	gtk_main_quit();
}

void on_Rename_clicked (GtkButton *b)
{
	gtk_widget_set_sensitive(applyBtn, FALSE);
	SrcPath = getTextFromTextWidget(srcPathText, srcPathBuf);
	DestPath = getTextFromTextWidget(destPathText, destPathBuf);
	SrcExt = getTextFromTextWidget(srcExtText, srcExtBuf);
	DestExt = getTextFromTextWidget(destExtText, destExtBuf);
	//Set_Status(1);
	printf("Entering the Rename Process\n");
	//testHello();
	Process_Rename();
//	printf("%s\n%s\n%s\n%s\n", SrcPath, SrcExt, DestPath, DestExt);
}

gboolean key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data) 
{
	gtk_widget_set_sensitive(applyBtn, TRUE);
	return FALSE;
}

char * getTextFromTextWidget(GtkWidget *Text, GtkTextBuffer *TextBuf)
{
	GtkTextIter Start;
	GtkTextIter End;
	char *retVal;
	retVal = (char *)malloc(1024 * sizeof(char));
	gtk_widget_set_sensitive(Text, FALSE);
	TextBuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Text));
	gtk_text_buffer_get_start_iter(TextBuf, &Start);
	gtk_text_buffer_get_end_iter(TextBuf, &End);
	retVal = gtk_text_buffer_get_text(TextBuf, &Start, &End, FALSE);
	gtk_widget_set_sensitive(Text, TRUE);
	return(retVal);
}

void Set_Status(int state)
{
	gchar *StatusText = (char *)malloc(1024 * sizeof(char));
	switch(state)
	{
		case 0:
			snprintf(StatusText, 1024 * sizeof(char), "%s%s%s%s%s", "<span foreground='red'>", PROCESS_FAILED, " ", ErrorDesc, "</span>");
			break;
		case 2:
			snprintf(StatusText, 1024 * sizeof(char), "%s%s%s", "<span foreground='green'>", PROCESS_COMPLETED, "</span>");
			break;
	}
	gtk_label_set_markup(GTK_LABEL(statusLabel), StatusText);
}

void on_SourceFileChooser_file_set(GtkFileChooserButton *f)
{
	SrcPath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
	gtk_text_buffer_set_text(srcPathBuf, SrcPath, -1);
}

void on_DestinationFileChooser_file_set(GtkFileChooserButton *f)
{
	DestPath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
	gtk_text_buffer_set_text(destPathBuf, DestPath, -1);
}

void Set_Status_Desc(char *StatDesc)
{
	gtk_label_set_markup(GTK_LABEL(statusLabel), StatDesc);
}

