#include <base.h>
#include <fstream>    // For ifstream
#include <sstream>    // For std::ostringstream

namespace start {
    void start() {
        std::ifstream ifile("./metadata.json");
        if(!ifile.is_open()){
            std::cerr<<"文件打开错误";
        }
        std::ostringstream buf;
        std::string filestr;
        char ch;
        while(buf && ifile.get(ch)){
            buf.put(ch);
        }
        filestr = buf.str();
        return;
    }
}
