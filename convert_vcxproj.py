import os
from pathlib import Path

# 시작 디렉터리 설정
start_directory = Path("./")

# .vcxproj 파일을 찾아 처리
for filepath in start_directory.rglob('*.vcxproj'):
    with open(filepath, 'r', encoding='utf-8') as file:
        content = file.read()

    patterns = [
        "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">",
        "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">"
    ]

    modified = False
    for pattern in patterns:
        index = content.find(pattern)
        if index != -1:
            index_end = content.find("</PropertyGroup>", index)
            if index_end != -1:
                insertion_point = index_end
                content = content[:insertion_point] + "  <PlatformToolset>v143</PlatformToolset>\n  " + content[insertion_point:]
                modified = True

    if modified:
        with open(filepath, 'w', encoding='utf-8') as file:
            file.write(content)
            print(f"Modified {filepath}")