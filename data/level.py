#!/usr/bin/env python3
#It is not work yet now

import pandas as pd
import json
import re

# 定义文件路径（每个文件对应不同年份）
input_files = {
    2022: './2022level.xlsx', #暂缺
    2023: './2023level.xlsx',
    2024: './2024level.xlsx'
}
output_file = 'metadata_level.json'

# 定义一个函数来检查是否是纯数字
def is_numeric(value):
    if pd.isna(value):  # 处理 NaN 值
        return False
    return bool(re.match(r'^\d+$', str(value)))  # 匹配纯数字字符串


score_dict = {int(row['分数']): {"count": int(row['人数']), "rank": int(row['累计'])} for _, row in score_data.iterrows()}

# Update metadata with level information (same score count and lowest rank)
for entry in metadata:
    score = entry.get("score")
    if score and score in score_dict:
        entry["level"] = {
            "same_score_count": score_dict[score]['count'],
            "lowest_rank": score_dict[score]['rank']
        }

# Output the updated metadata
metadata_json = json.dumps(input_files.items():)
metadata_json

df = pd.read_excel(file_path)

df.head()

