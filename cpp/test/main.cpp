#include <base.h>
#include <data.h>

int main() {
    const char* filePath = "../../metadata.json";
    view::view(filePath);
    search::search("辽宁科技大学", "电气工程及其自动化");
    return 0;
}
