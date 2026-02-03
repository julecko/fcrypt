#include "gui/gui_main.h"
#include "cli/cli_args.h"
#include "file_process.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Browser.H>
#include <FL/fl_ask.H>

#include <vector>
#include <string>
#include <sodium.h>

static Fl_Input *password_input;
static Fl_Browser *file_browser;
static cli_args_t *global_args;
static cli_args_action_t global_action;

void on_encrypt(Fl_Widget*, void*) {
    const char *password = password_input->value();

    if (!password || !*password) {
        fl_alert("Password required");
        return;
    }

    std::vector<std::string> selected_files;

    for (int i = 1; i <= file_browser->size(); i++) {
        if (file_browser->selected(i)) {
            selected_files.push_back(file_browser->text(i));
        }
    }

    if (selected_files.empty()) {
        fl_alert("No files selected");
        return;
    }

    for (auto &file : selected_files) {
        process_file(file.c_str(), password, global_action == ACTION_ENCRYPT);
    }

    fl_message("Selected files processed successfully!");
}

int gui_main(cli_args_t *args, cli_args_action_t action) {
    global_args = args;
    global_action = action;

    if (sodium_init() < 0) {
        fl_alert("libsodium init failed");
        return 1;
    }

    Fl_Window win(500, 350, "fcrypt");

    file_browser = new Fl_Browser(20, 20, 460, 200);
    file_browser->type(FL_MULTI_BROWSER);

    for (int i = 0; i < args->file_paths_length; i++) {
        file_browser->add(args->file_paths[i]);
    }

    password_input = new Fl_Input(120, 240, 250, 30, "Password:");
    password_input->type(FL_SECRET_INPUT);

    Fl_Button encrypt_btn(200, 290, 100, 30, "Encrypt");
    encrypt_btn.callback(on_encrypt);

    win.end();
    win.show();

    return Fl::run();
}
