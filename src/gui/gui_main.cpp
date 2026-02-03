#include "gui/gui_main.h"
#include "cli/cli_args.h"
#include "file_process.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Scroll.H>
#include <FL/fl_ask.H>

#include <vector>
#include <string>
#include <sstream>
#include <sodium.h>
#include <FL/Fl_Browser.H>

static Fl_Input *password_input;
static Fl_Multiline_Output *files_output;
static cli_args_t *global_args;
static cli_args_action_t global_action;

void on_encrypt(Fl_Widget*, void*) {
    const char *password = password_input->value();

    if (!password || !*password) {
        fl_alert("Password required");
        return;
    }

    for (int i = 0; i < global_args->file_paths_length; i++) {
        process_file(global_args->file_paths[i], password, global_action == ACTION_ENCRYPT);
    }

    fl_message("All files processed successfully!");
}

int gui_main(cli_args_t *args, cli_args_action_t action) {
    global_args = args;
    global_action = action;

    if (sodium_init() < 0) {
        fl_alert("libsodium init failed");
        return 1;
    }

    Fl_Window win(500, 350, "fcrypt");

    Fl_Box *files_label = new Fl_Box(20, 0, 60, 20, "Files:");
    files_label->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

    Fl_Browser *list = new Fl_Browser(20, 20, 460, 200);
    list->type(FL_HOLD_BROWSER);

    for (int i = 0; i < args->file_paths_length; i++) {
        list->add(args->file_paths[i]);
    }

    password_input = new Fl_Input(120, 240, 250, 30, "Password:");
    password_input->type(FL_SECRET_INPUT);

    const char *action_type = (action == ACTION_ENCRYPT) ? "Encrypt" : "Decrypt";

    Fl_Button encrypt_btn(200, 290, 100, 30, action_type);
    encrypt_btn.callback(on_encrypt);

    win.end();
    win.show();

    int exit_code = Fl::run();
    free_cli_args(args);
    return exit_code;
}
