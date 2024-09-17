#include <data.h>
#include <base.h>
#include <algorithm> // std::min

std::map<int, int> minScoresByYear;

namespace search {
    void search(const std::string& university, const std::string& profession) {
        bool found = false;

        for (const auto& uni : universityData) {
            // 精确匹配大学名称和专业
            if (uni.name == university && uni.profession == profession) {
            // 过滤年份在2022-2024之间
                if (uni.year >= 2022 && uni.year <= 2024) {
                    found = true;
                    if (uni.score != "null" && !uni.score.empty()) {
                        int scoreInt = std::stoi(uni.score);  // 转换 score 为 int

                        // 更新每年的最低投档线
                        if (minScoresByYear.find(uni.year) == minScoresByYear.end() || scoreInt < minScoresByYear[uni.year]) {
                            minScoresByYear[uni.year] = scoreInt;
                        }
                    }
                }
            }
        }
        // 输出每年的最低投档线
        if (found) {
            std::cout << university << profession << "2022-2024年最低投档分:" << std::endl;
            for (int year = 2022; year <= 2024; ++year) {
                std::cout << "Year " << year << ": ";
                auto it = minScoresByYear.find(year);
                if (it != minScoresByYear.end()) {
                    std::cout << it->second << std::endl;
                } else {
                    std::cout << "No data" << std::endl;
                }
            }
        } else {
            std::cout << "没有找到匹配的大学或专业。" << std::endl;
        }
    }
}
