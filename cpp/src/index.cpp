#include <base.h>

namespace start {
    void start() {
        const char* filePath = "./metadata.json";
        view::view(filePath);
        // 用户输入大学和专业
        std::string university, profession;
        std::cout << "请输入大学名称: ";
        std::getline(std::cin, university);
        std::cout << "请输入专业名称: ";
        std::getline(std::cin, profession);

        // 执行精确搜索
        search::search(university, profession);
        return;
    }
}
