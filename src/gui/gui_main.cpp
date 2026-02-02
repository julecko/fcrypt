#include "gui/gui_main.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>

#include <sodium.h>
#include "file_process.h"

static Fl_Input *password_input;

void on_encrypt(Fl_Widget*, void*) {
    const char *password = password_input->value();

    if (!password || !*password) {
        fl_alert("Password required");
        return;
    }

}

int gui_main(void) {
    if (sodium_init() < 0) {
        fl_alert("libsodium init failed");
        return 1;
    }

    Fl_Window win(400, 200, "fcrypt");

    password_input = new Fl_Input(120, 40, 200, 30, "Password:");
    password_input->type(FL_SECRET_INPUT);

    Fl_Button encrypt_btn(150, 100, 100, 30, "Encrypt");
    encrypt_btn.callback(on_encrypt);

    win.end();
    win.show();

    return Fl::run();
}