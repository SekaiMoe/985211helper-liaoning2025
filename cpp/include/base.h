#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <json/json.h>

namespace projsignal {
    void register_signal(void);
}

namespace searchjson {
    void searchjson(const char* filePath);
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

#endif
