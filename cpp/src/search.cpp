#include <data.h>
#include <base.h>

std::map<int, int> minScoresByYear;

namespace search {
    inline double predictScore(int year1, int score1, int year2, int score2, int year3, int score3, int predictYear) {
        // 使用两两年份之间的分数变化计算斜率（delta score / delta year）
        double slope1 = (double)(score2 - score1) / (year2 - year1);
        double slope2 = (double)(score3 - score2) / (year3 - year2);

        // 取平均斜率
        double avgSlope = (slope1 + slope2) / 2.0;

        // 使用线性回归公式 y = mx + b 预测分数
        // b（截距） = score3 - slope * year3
        double intercept = score3 - avgSlope * year3;

        // 预测 2025 年的分数
        double predictedScore = avgSlope * predictYear + intercept;
        return predictedScore;
    }
    #ifdef WEBUI
    void search(const std::string& university, const std::string& profession, crow::json::wvalue& response) {
        bool found = false;
        bool is985 = false;
        bool is211 = false;

        // 存储分数
        std::map<int, int> minScoresByYear;

        // 搜索大学和专业信息
        for (const auto& uni : universityData) {
            if (uni.name == university && uni.profession == profession) {
                found = true;
                is985 = uni.is985;
                is211 = uni.is211;

                if (uni.year >= 2022 && uni.year <= 2024) {
                    if (minScoresByYear.find(uni.year) == minScoresByYear.end() || std::stoi(uni.score) < minScoresByYear[uni.year]) {
                        minScoresByYear[uni.year] = std::stoi(uni.score);
                    }
                }
            }
        }

        if (found) {
            response["university"] = university;
            response["profession"] = profession;

            // 输出每年的最低投档线
            crow::json::wvalue::list scoreList;
            for (int year = 2022; year <= 2024; ++year) {
                auto it = minScoresByYear.find(year);
                if (it != minScoresByYear.end()) {
                    scoreList.emplace_back(crow::json::wvalue{{"year", year}, {"score", it->second}});
                } else {
                    scoreList.emplace_back(crow::json::wvalue{{"year", year}, {"score", "No data"}});
                }
            }
            response["scores"] = std::move(scoreList);

            // 计算平均分
            int totalScore = 0;
            if (minScoresByYear.size() == 3) {
                for (const auto& entry : minScoresByYear) {
                    totalScore += entry.second;
                }
                int averageScore = static_cast<int>(std::ceil(static_cast<double>(totalScore) / 3));
                response["average_score"] = averageScore;

                // 预测 2025 年的分数
                int score2022 = minScoresByYear[2022];
                int score2023 = minScoresByYear[2023];
                int score2024 = minScoresByYear[2024];
                double predictedScore = predictScore(2022, score2022, 2023, score2023, 2024, score2024, 2025);
                response["predicted_2025_score"] = predictedScore;
            } else {
                response["average_score"] = "无法计算三年平均分，数据不足。";
            }

            // 985/211 信息
            response["is985"] = is985 ? "是" : "否";
            response["is211"] = is211 ? "是" : "否";

        } else {
            response["error"] = "没有找到匹配的大学或专业。";
        }
    }
    #else
    void search(const std::string& university, const std::string& profession) {
        bool is985 = false;
        bool is211 = false;
        bool found = false;

/*        for (const auto& uni : universityData) {
            // 精确匹配大学名称和专业
            if (uni.name == university && uni.profession == profession) {
            // 过滤年份在2022-2024之间
                if (uni.year >= 2022 && uni.year <= 2024) {
                    found = true;
                    is985 = uni.is985;
                    is211 = uni.is211;
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
*/
        for (const auto& uni : universityData) {
            if (uni.name == university && uni.profession == profession) {
                if (uni.year >= 2022 && uni.year <= 2024) {
                    found = true;
                    is985 = uni.is985;
                    is211 = uni.is211;

                    if (uni.score != "null" && !uni.score.empty()) {
                        int scoreInt = std::stoi(uni.score);
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
            int totalScore = 0;  // 用于计算总分
            int count = 0;
            for (int year = 2022; year <= 2024; ++year) {
                std::cout << "Year " << year << ": ";
                auto it = minScoresByYear.find(year);
                if (it != minScoresByYear.end()) {
                    //std::cout << it->second << std::endl;
                    totalScore += it->second;  // 累加分数
                    ++count;
                    auto it = minScoresByYear.find(year);
                    std::cout << it->second << std::endl;
                } else {
                    std::cout << "No data" << std::endl;
                }
            }

            if (count == 3) {
                // double averageScore = static_cast<double>(totalScore) / count;
                double averageScore = static_cast<double>(totalScore) / count;
                int roundedAverageScore = static_cast<int>(std::ceil(averageScore));  // 向上取整
                std::cout << "近三年平均分(向上取整):" << roundedAverageScore << std::endl;
            } else {
                std::cout << "无法计算三年平均分，原因：某一年数据不足。" << std::endl;
            }

            if (minScoresByYear.size() == 3) {
                int score2022 = minScoresByYear[2022];
                int score2023 = minScoresByYear[2023];
                int score2024 = minScoresByYear[2024];

                // 预测 2025 年的分数线
                double predictedScore = predictScore(2022, score2022, 2023, score2023, 2024, score2024, 2025);
                std::cout << "Year 2025 (仅供参考): " << predictedScore << std::endl;

            } else {
                std::cout << "无法计算2025年预测分数，原因：某一年数据不足。" << std::endl;
                std::cout << "\n这可能是因为该专业近三年来未在辽宁省招生的缘故，请谨慎考虑该专业！" << std::endl;
                return;
            }

            std::cout << "\n该学校是";
            if (is985) {
                std::cout << " 985 ";
            }
            if (is211) {
                std::cout << " 211 ";
            }
            if (!is985 && !is211) {
                std::cout << " 非 985/211 ";
            }
            std::cout << "大学。" << std::endl;

        } else {
            std::cout << "没有找到匹配的大学或专业。" << std::endl;
            exit(2);
        }
    }
    #endif

    void listProfessions(const std::string& university, std::vector<std::string>& professions) {
        for (const auto& uni : universityData) {
            if (uni.name == university) {
                if (std::find(professions.begin(), professions.end(), uni.profession) == professions.end()) {
                    professions.push_back(uni.profession);
                }
            }
        }
    }
}
