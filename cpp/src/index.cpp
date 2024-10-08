#include <base.h>
#include <data.h>

namespace startcli {
    void startcli() {
        const char* filePath = "./metadata.json";
        view::view(filePath);
        // 用户输入大学和专业
        std::string university, profession;
        std::cout << "请输入大学名称: ";
        std::getline(std::cin, university);
        #ifndef WEBI
        std::vector<std::string> professions;
        search::listProfessions(university, professions);
        #else
        search::listProfessions(university, professions, response);
        exit(0);
        #endif

        if (professions.empty()) {
            std::cout << "没有找到该大学。" << std::endl;
            exit(1);
        }

        // 列出专业并让用户选择
        std::cout << "\n该大学的专业如下，请选择专业编号:" << std::endl;
        for (size_t i = 0; i < professions.size(); ++i) {
            std::cout << i + 1 << ": " << professions[i] << std::endl;
        }

        int choice;
        while (true) {
            std::cout << "\n请输入专业编号，输入0退出: ";
            std::cin >> choice;

            if (choice == 0) {
                exit(0);  // 用户选择退出
            }

            if (std::cin.fail()) {
                std::cin.clear();  // 清除错误状态
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 丢弃无效输入
                std::cerr << "无效的输入，请重新输入。" << std::endl;
                continue;  // 重新提示用户输入
            }

            if (choice < 1 || choice > static_cast<int>(professions.size())) {
                std::cerr << "无效的选择，请重新输入。" << std::endl;
            } else {
                break;  // 输入有效，跳出循环
            }
        }
        //profession = professions[choice - 1];  // 直接赋值
        profession = professions[static_cast<std::vector<std::string>::size_type>(choice - 1)];
        // 执行精确搜索
        //std::string profession = professions[choice - 1];
        #ifndef WEBUI
        search::search(university, profession);
        #else
        exit(0);
        #endif
        return;
    }
}
