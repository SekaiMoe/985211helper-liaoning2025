#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>

#include <base.h>

struct UniversityInfo {
    int year;
    std::string name;
    std::string code;
    std::string profession;
    int is985;
    int is211;
    std::string score; // 存储为字符串以处理 null 值
};

extern std::vector<UniversityInfo> universityData; // 全局存储 JSON 数据

#endif
