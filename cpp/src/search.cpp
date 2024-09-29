#include <data.h>
#include <base.h>
#include <algorithm> // std::min

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
                std::cout << "无法计算三年平均分，数据不足。" << std::endl;
            }

            if (minScoresByYear.size() == 3) {
                int score2022 = minScoresByYear[2022];
                int score2023 = minScoresByYear[2023];
                int score2024 = minScoresByYear[2024];

                // 预测 2025 年的分数线
                double predictedScore = predictScore(2022, score2022, 2023, score2023, 2024, score2024, 2025);
                std::cout << "Year 2025 (仅供参考): " << predictedScore << std::endl;

            } else {
                std::cout << "数据不足。" << std::endl;
                exit(2);
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
            exit(1);
        }
    }

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
