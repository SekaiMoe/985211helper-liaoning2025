#include <base.h>

namespace searchjson {
    void searchjson(const char* filePath) {
        for (auto it : root[""]) {
            std::cout << it["year"].asString() << std::endl;
            std::cout << it["name"].asString() << std::endl;
            std::cout << it["code"].asString() << std::endl;
            std::cout << it["is985"].asString() << std::endl;
            std::cout << it["is211"].asString() << std::endl;
            std::cout << it["score"].asString() << std::endl;
        }
    }
}
