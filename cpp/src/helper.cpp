#include <base.h>
#include <version.h>
#include "qt/window.h"
#include <QScreen>
#include <cstdlib>  // for getenv()

int main(int argc, char *argv[]) {
    projsignal::register_signal();

    if (argc < 2) {
        std::cout << "Error: No inputs.\n"
                  << "Please use 'helper -h' to learn how to use.\n";
        exit(1);
    } else if (argc > 4) {
        std::cout << "Error: Too many inputs.\n";
        exit(2);
    }

    // Main options
    for (int i = 1; i < argc; ++i) {
        char *pchar = argv[i];
        switch (pchar[0]) {
            case '-': {
                switch (pchar[1]) {
                    case 'v':
                        version::version();
                        exit(0);
                    case 'h':
                        usage::usage();
                        exit(0);
                    case 's': {
			    #ifdef APPUI
			        #ifdef WEBUI
			            webui::start();
			        #else
				        #ifdef GUI
				            Window::runGUI();
				        #else
				            startcli::startcli();
					#endif
			        #endif
			#endif
                        exit(0);
                    }
                    default:
                        std::cerr << version::prog_name << ": Error: Unrecognized option: " << pchar << std::endl;
                        exit(254);
                }
                break;
            }
        }
    }
}

