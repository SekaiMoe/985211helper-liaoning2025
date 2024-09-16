#include <base.h>

namespace usage {
    void usage() {
        std::cout << "Usage: helper [OPTION]... \n"
        << "List information about the FILEs (the current directory by default).\n\n"
        << "-h               show the program usage.\n"
        << "-s               start the program.\n"
        << "-v               show the program version.\n\n"
        << "N: this is a unstable app.\n";
        return;
    }
}
