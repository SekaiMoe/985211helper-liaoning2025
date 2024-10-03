#include <base.h>

namespace run {
void startcheck() {
    #ifdef APPUI
        #if APPUI == "WEBUI"
            webui::start();
        #elif APPUI == "GUI"
            Window::runGUI();
        #elif APPUI == "TUI"
            startcli::startcli();
        #else
            // Fallback in case APPUI has an unexpected value
            std::cerr << "Error: Unsupported UI type." << std::endl;
        #endif
    #else
        // 当没有定义 APPUI 时的默认行为
        startcli::startcli();
    #endif
}

