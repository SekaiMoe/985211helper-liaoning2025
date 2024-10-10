#include <data.h>

std::vector<UniversityInfo> universityData;

namespace view {
    void view(const char* filePath) {
        // 打开文件
        std::ifstream file(filePath, std::ifstream::binary);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << filePath << std::endl;
            exit(2);
        }

        // 解析 JSON 文件
        Json::Value jsonData;
        Json::CharReaderBuilder readerBuilder;
        std::string errs;

        if (!Json::parseFromStream(readerBuilder, file, &jsonData, &errs)) {
            std::cerr << "解析 JSON 失败: " << errs << std::endl;
            exit(1);
        }

        universityData.clear();

        // 遍历 JSON 数组
        for (const auto& it : jsonData) {
            UniversityInfo uniInfo;
            uniInfo.year = it["year"].asInt();
            uniInfo.name = it["name"].asString();
            uniInfo.code = it["code"].asString();
            uniInfo.profession = it["profession"].asString();
            uniInfo.is985 = it["is985"].asInt();
            uniInfo.is211 = it["is211"].asInt();
            uniInfo.score = it["score"].isNull() ? "null" : std::to_string(it["score"].asInt());
            universityData.push_back(uniInfo);
        }
        std::cout << "json loaded successfully.\n";
    }
}

