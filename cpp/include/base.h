#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>
#include <algorithm>
#include <json/json.h>

#ifdef WEBUI
#include "../src/web/crow.h"
#endif

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

namespace webui {
    void start();
}

namespace startcli {
    void startcli();
}

namespace search {
    #ifdef WEBUI
    void search(const std::string& university, const std::string& profession, crow::json::wvalue& response);
    #else
    void search(const std::string& university, const std::string& profession);
    #endif
    void listProfessions(const std::string& university, std::vector<std::string>& professions);
}

#endif
