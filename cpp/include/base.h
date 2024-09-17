#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <fstream>
#include <json/json.h>

namespace projsignal {
    void register_signal(void);
}

namespace view {
    void view(const char* filePath);
}

namespace version {
    void version();
}

namespace usage {
    void usage();
}

namespace start {
    void start();
}

namespace search {
    void search(const std::string& university, const std::string& profession);
}

#endif
