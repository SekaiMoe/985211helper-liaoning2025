#include <base.h>
#include <data.h>

int main() {
    const char* filePath = "../../metadata.json";
    view::view(filePath);

    const char* university = "辽宁工程技术大学";
    std::vector<std::string> professions;

    // 调用 listProfessions 获取专业列表
    search::listProfessions(university, professions);

    // 输出专业列表
    std::cout << "\n该大学的专业如下:" << std::endl;
    for (size_t i = 0; i < professions.size(); ++i) {
        std::cout << i + 1 << ": " << professions[i] << std::endl;
    }

    // 进行专业查询
    search::search("辽宁科技大学", "电气工程及其自动化");

    return 0;
}

