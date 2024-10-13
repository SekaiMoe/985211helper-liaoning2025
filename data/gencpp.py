#!/usr/bin/env python3
import json

with open('metadata.json', 'r', encoding='utf-8') as file:
    json_content = file.read()

cpp_string = f'const char* embedded_metadata_json = R"({json_content})";'

with open('metadata.cpp', 'w', encoding='utf-8') as output_file:
    output_file.write(cpp_string)

print("metadata.cpp 已生成。")

