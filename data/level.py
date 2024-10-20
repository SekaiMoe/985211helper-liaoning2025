#!/usr/bin/env python3

import pandas as pd
import json
import re

# Define file paths (each file corresponds to a different year)
input_files = {
    2022: './2022level.xlsx',  # Temporarily missing
    2023: './2023level.xlsx',
    2024: './2024level.xlsx'
}
output_file = 'metadata_level.json'

# Define a function to check if a value is a pure number
def is_numeric(value):
    if pd.isna(value):  # Handle NaN values
        return False
    return bool(re.match(r'^\d+$', str(value)))  # Match pure numeric strings

# Read score data and metadata from the respective Excel files
# Assuming the structure of the files is known; replace 'file_path' with actual file logic
score_data = pd.read_excel(input_files[2023])  # Example for the year 2023
metadata = [...]  # Populate this list with your metadata structure

# Create a score_dict mapping scores to count and rank
score_dict = {
    int(row['分数']): {
        "count": int(row['人数']),
        "rank": int(row['累计'])
    }
    for _, row in score_data.iterrows()
}

# Update metadata with level information (same score count and lowest rank)
for entry in metadata:
    score = entry.get("score")
    if score and score in score_dict:
        entry["level"] = {
            "same_score_count": score_dict[score]['count'],
            "lowest_rank": score_dict[score]['rank']
        }

# Output the updated metadata to a JSON file
with open(output_file, 'w') as f:
    json.dump(metadata, f, ensure_ascii=False, indent=4)

# Example: Read another Excel file if needed
# file_path = input_files[2024]  # Use for the year 2024
# df = pd.read_excel(file_path)
# print(df.head())  # Output the first few rows of the DataFrame
