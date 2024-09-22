#include <base.h>
#include <data.h>

namespace start {
    void start() {
        const char* filePath = "./metadata.json";
        view::view(filePath);
        // 用户输入大学和专业
        std::string university, profession;
        std::cout << "请输入大学名称: ";
        std::getline(std::cin, university);

        std::vector<std::string> professions;
        search::listProfessions(university, professions);

        if (professions.empty()) {
            std::cout << "没有找到该大学的相关专业。" << std::endl;
            exit(1);
        }

        // 列出专业并让用户选择
        std::cout << "\n该大学的专业如下，请选择专业编号:" << std::endl;
        for (size_t i = 0; i < professions.size(); ++i) {
            std::cout << i + 1 << ": " << professions[i] << std::endl;
        }

        int choice;
        std::cout << "\n请输入专业编号: ";
        std::cin >> choice;

        if (choice < 1 || choice > static_cast<int>(professions.size())) {
            std::cerr << "无效的选择。" << std::endl;
            exit(2);
        }
        profession = professions[choice - 1];  // 直接赋值
        // 执行精确搜索
        //std::string profession = professions[choice - 1];
        search::search(university, profession);
        return;
    }
}
