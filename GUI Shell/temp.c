void run_command(GtkWidget *widget, gpointer data)
{
    const gchar *command = gtk_entry_get_text(GTK_ENTRY(data));
    update_history(command);

    // Initialize your shell
    initsh();

    // Create a source structure for the command
    struct source_s src;
    src.buffer = command;
    src.bufsize = strlen(command);
    src.curpos = INIT_SRC_POS;

    // Parse and execute the command using your shell logic
    parse_and_execute(&src);
    char *output = NULL;
    GError *error = NULL;
    gboolean result = g_spawn_sync(
        NULL,                                       // working directory
        (char *[]){"/bin/sh", "-c", command, NULL}, // command and arguments
        NULL,                                       // environment
        G_SPAWN_SEARCH_PATH,                        // flags
        NULL,                                       // child setup function
        NULL,                                       // user data
        &output,                                    // standard output
        NULL,                                       // standard error
        NULL,                                       // exit status
        &error);

    if (result)
    {
        // Successfully ran the command, update the output_label
        gtk_label_set_text(GTK_LABEL(output_label), output);
        g_free(output);
    }
    else
    {
        // Handle error, you may want to display an error message
        g_printerr("Error executing command: %s\n", error->message);
        g_error_free(error);
    }
}