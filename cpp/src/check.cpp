#include <base.h>

namespace run {
    void startcheck() {
    #ifdef WEBUI_INT
        webui::start();
    #else
    #ifdef QTUI_INT
        Window::runGUI();
    #else
        startcli::startcli();
    #endif
    #endif
}

