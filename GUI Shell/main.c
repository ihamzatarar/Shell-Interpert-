#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include "shell.h"
#include "shell.h"
#include "source.h"
#include "parser.h"
#include "executor.h"
#include <fcntl.h>

void run_command(GtkWidget *widget, gpointer data);
void exit_program(GtkWidget *widget, gpointer data);
void update_history(const char *command);
void save_log(GtkWidget *widget, gpointer data);
void show_help(GtkWidget *widget, gpointer data);

GtkWidget *output_label;
GtkTextBuffer *history_buffer;

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    // Create the main window with a grid layout
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Command Interpreter GUI");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid layout
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    // Create a scrolled window for the command history
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);

    // Create a text view for the command history
    GtkWidget *history_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(history_view), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), history_view);

    // Get the text buffer for the history view
    history_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(history_view));

    // Create an entry field
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter command here");

    // Create a button to run the command
    GtkWidget *run_button = gtk_button_new_with_label("Run Command");
    g_signal_connect(run_button, "clicked", G_CALLBACK(run_command), entry);

    // Create a "Save Log" button
    GtkWidget *save_log_button = gtk_button_new_with_label("Save Log");
    g_signal_connect(save_log_button, "clicked", G_CALLBACK(save_log), window);

    // Create a "Help" button
    GtkWidget *help_button = gtk_button_new_with_label("Help");
    g_signal_connect(help_button, "clicked", G_CALLBACK(show_help), window);

    // Create an exit button
    GtkWidget *exit_button = gtk_button_new_with_label("Exit");
    g_signal_connect(exit_button, "clicked", G_CALLBACK(exit_program), window);

    // Create a label for displaying output
    output_label = gtk_label_new("Output will be displayed here");

    // Create labels for input and history
    GtkWidget *input_label = gtk_label_new("Input:");
    GtkWidget *history_label = gtk_label_new("History:");
    GtkWidget *output_label_text = gtk_label_new("Output:");

    // Set styles for some widgets
    gtk_widget_set_name(entry, "entryStyle");
    gtk_widget_set_name(run_button, "buttonStyle");
    gtk_widget_set_name(save_log_button, "buttonStyle");
    gtk_widget_set_name(help_button, "buttonStyle");
    gtk_widget_set_name(exit_button, "buttonStyle");
    gtk_widget_set_name(output_label, "labelStyle");
    gtk_widget_set_name(output_label_text, "labelStyle");

    // Attach widgets to the grid layout with dynamic sizing
    gtk_grid_attach(GTK_GRID(grid), history_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 1, 5, 1);
    gtk_grid_attach(GTK_GRID(grid), output_label_text, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), output_label, 0, 3, 5, 1);
    gtk_grid_attach(GTK_GRID(grid), input_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), run_button, 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), save_log_button, 3, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), help_button, 4, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), exit_button, 5, 5, 1, 1);

    // Set properties for dynamic sizing
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_widget_set_halign(entry, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(output_label, TRUE);
    gtk_widget_set_halign(output_label, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(scrolled_window, TRUE);
    gtk_widget_set_halign(scrolled_window, GTK_ALIGN_FILL);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_widget_set_valign(scrolled_window, GTK_ALIGN_FILL);

    // Add the grid to the main window
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Load CSS for styling
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
                                    "entryStyle { background-color: #f0f0f0; }"
                                    "buttonStyle { background-color: #4CAF50; color: white; }"
                                    "labelStyle { color: #333333; }",
                                    -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}

void exit_program(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

void update_history(const char *command)
{
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(history_buffer, &iter);

    gtk_text_buffer_insert(history_buffer, &iter, command, -1);
    gtk_text_buffer_insert(history_buffer, &iter, "\n", -1);
}

void save_log(GtkWidget *widget, gpointer data)
{
    time_t rawtime;
    struct tm *info;
    char buffer[80];

    time(&rawtime);

    info = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "logs/log_%Y%m%d%H%M%S.txt", info);

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(history_buffer, &start, &end);

    FILE *fp = fopen(buffer, "w");
    if (fp == NULL)
    {
        perror("Error opening log file");
        return;
    }

    char *text = gtk_text_buffer_get_text(history_buffer, &start, &end, FALSE);
    fprintf(fp, "%s", text);
    fclose(fp);

    g_free(text);

    // Optionally, notify the user that the log has been saved
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Log saved to %s", buffer);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_help(GtkWidget *widget, gpointer data)
{
    // Create a simple help dialog
    GtkWidget *help_dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "Welcome to the Command Interpreter GUI!\n\n"
                                                    "1. Enter a command in the input box and click 'Run Command' to execute it.\n"
                                                    "2. The output will be displayed in the output box.\n"
                                                    "3. Click 'Save Log' to save the command history to a text file.\n"
                                                    "4. Click 'Help' to view this help message.\n"
                                                    "5. Click 'Exit' to close the application.\n");
    gtk_dialog_run(GTK_DIALOG(help_dialog));
    gtk_widget_destroy(help_dialog);
}

void run_command(GtkWidget *widget, gpointer data)
{
    const gchar *command = gtk_entry_get_text(GTK_ENTRY(data));
    update_history(command);

    // Save the current standard output
    int stdout_backup = dup(fileno(stdout));

    // Create a pipe to redirect standard output
    int output_pipe[2];
    if (pipe(output_pipe) == -1)
    {
        perror("Error creating pipe");
        return;
    }

    // Redirect standard output to the write end of the pipe
    dup2(output_pipe[1], fileno(stdout));
    close(output_pipe[1]);

    // Initialize your shell
    initsh();

    // Create a source structure for the command
    struct source_s src;
    src.buffer = command;
    src.bufsize = strlen(command);
    src.curpos = INIT_SRC_POS;

    // Parse and execute the command using your shell logic
    parse_and_execute(&src);

    // Restore the original standard output
    dup2(stdout_backup, fileno(stdout));

    // Close the read end of the pipe
    close(output_pipe[1]);

    // Read the captured output from the pipe
    char captured_output[1024];
    ssize_t bytesRead = read(output_pipe[0], captured_output, sizeof(captured_output) - 1);
    captured_output[bytesRead] = '\0';

    // Check if the captured output is valid UTF-8
    if (!g_utf8_validate(captured_output, -1, NULL))
    {
        // Handle the case where the string is not valid UTF-8
        // You might want to perform encoding conversion or handle it in some way
        g_warning("Invalid UTF-8 string in captured output");
    }

    // Display the captured output in the GUI label
    gtk_label_set_text(GTK_LABEL(output_label), captured_output);
}

int parse_and_execute(struct source_s *src)
{
    skip_white_spaces(src);

    struct token_s *tok = tokenize(src);

    if (tok == &eof_token)
    {
        return 0;
    }

    while (tok && tok != &eof_token)
    {
        struct node_s *cmd = parse_simple_command(tok);

        if (!cmd)
        {
            break;
        }

        do_simple_command(cmd);
        free_node_tree(cmd);
        tok = tokenize(src);
    }
    return 1;
}