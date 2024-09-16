#!/usr/bin/env python3

import pandas as pd
import json
import re

# 定义文件路径（每个文件对应不同年份）
input_files = {
    2022: './2022.xlsx',
    2023: './2023.xlsx',
    2024: './2024.xlsx'
}
output_file = 'metadata.json'

# 定义列名
columns = ['院校编号', '招生院校', '专业编号', '招生专业', '投档最低分', '投档最低分相同考生排序项(一)',
           '投档最低分相同考生排序项(二)', '投档最低分相同考生排序项(三)', '投档最低分相同考生排序项(四)',
           '投档最低分相同考生排序项(五)', '投档最低分相同考生排序项(六)', '志愿号']

# 定义985和211大学名单
university_985 = [
    "北京大学", "清华大学", "浙江大学", "上海交通大学", "复旦大学", "南京大学",
    "武汉大学", "四川大学", "中山大学", "山东大学", "吉林大学", "南开大学",
    "北京师范大学", "华东师范大学", "西安交通大学", "天津大学", "中国人民大学",
    "中国科学技术大学", "东南大学", "天津大学", "哈尔滨工业大学", "北京航空航天大学",
    "北京理工大学", "国防科技大学", "华南理工大学", "大连理工大学", "华中科技大学",
    "中南大学", "电子科技大学", "西北工业大学", "东北大学", "重庆大学",
    "湖南大学", "中国农业大学", "厦门大学", "中国海洋大学", "兰州大学",
    "中央民族大学", "西北农林科技大学"
]

university_211 = [
    "南京师范大学", "东北师范大学", "华中师范大学", "华南师范大学", "陕西师范大学",
    "湖南师范大学", "海军军医大学", "空军军医大学", "北京中医药大学", "中国药科大学",
    "华中农业大学", "南京农业大学", "四川农业大学", "东北农业大学", "中央财经大学",
    "对外经济贸易大学", "上海财经大学", "中南财经政法大学", "西南财经大学", "北京外国语大学",
    "上海外国语大学", "中国政法大学", "中国传媒大学", "北京交通大学", "中国矿业大学",
    "北京科技大学", "北京邮电大学", "西安电子科技大学", "哈尔滨工程大学", "西南交通大学",
    "南京理工大学", "中国石油大学", "河海大学", "江南大学", "华东理工大学",
    "中国地质大学", "武汉理工大学", "东华大学", "北京工业大学", "华北电力大学",
    "北京化工大学", "合肥工业大学", "南京航空航天大学", "太原理工大学", "长安大学",
    "河北工业大学", "大连海事大学", "西北大学", "暨南大学", "苏州大学",
    "南昌大学", "辽宁大学", "贵州大学", "延边大学", "中央音乐学院",
    "北京体育大学", "广西大学", "安徽大学", "海南大学", "内蒙古大学",
    "石河子大学", "宁夏大学", "青海大学", "云南大学", "郑州大学",
    "新疆大学", "上海大学", "南昌大学", "福州大学"
]

# 定义一个函数来检查是否是纯数字
def is_numeric(value):
    if pd.isna(value):  # 处理 NaN 值
        return False
    return bool(re.match(r'^\d+$', str(value)))  # 匹配纯数字字符串

# 统一处理数据，添加985、211和中外合作办学信息
def process_data(data, year):
    data['985'] = data['招生院校'].apply(lambda x: 1 if x in university_985 else 0)
    data['211'] = data['招生院校'].apply(lambda x: 1 if x in university_211 else 0)
    data['中外合作'] = data['招生院校'].apply(lambda x: 1 if '(中外合作办学)' in str(x) else 0)
    
    # 去掉 "(中外合作办学)" 字符串
    data['招生院校'] = data['招生院校'].apply(lambda x: str(x).replace('(中外合作办学)', ''))
    
    # 构建导出的 JSON 数据
    export = []
    for i, row in data.iterrows():
        code = row['院校编号']
        # 只转换数字编号
        if is_numeric(code):
            code = int(code)
        else:
            code = str(code)  # 如果不是数字，则保留为字符串

        export.append({
            "year": year,  # 添加年份信息
            "code": code,
            "name": str(row['招生院校']),
            "profession": str(row['招生专业']),
            "is985": int(row['985']),
            "is211": int(row['211']),
            "score": int(row['投档最低分']) if pd.notna(row['投档最低分']) else None,
        })
    
    return export

# 处理每个年份的文件
final_export = []
for year, file_path in input_files.items():
    data = pd.read_excel(file_path, skiprows=2, names=columns)
    processed_data = process_data(data, year)
    final_export.extend(processed_data)

# 将合并后的数据保存到 JSON 文件
with open(output_file, 'w', encoding='utf-8') as file:
    json.dump(final_export, file, ensure_ascii=False, indent=4)

print(f"数据已成功导出到 {output_file}")

