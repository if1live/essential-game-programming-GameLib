'''
chatgpt
하위 디렉토리에 있는 cpp, h 파일의 현재 인코딩이 shift-jis인데 이걸 utf-8로 바꾸는 스크립트 짜줘
'''

import os
import chardet
from pathlib import Path

# 변경할 디렉토리 경로를 설정합니다.
path = "./"

def convert_encoding(file_path, from_encodings, to_encoding='utf-8'):
    with open(file_path, 'rb') as file:
        content = file.read()

    # 파일의 현재 인코딩을 감지하고, 원하는 인코딩으로 변환합니다.
    current_encoding = chardet.detect(content)['encoding']
    if current_encoding in from_encodings:
        # 인코딩 모르겠으면 무식하게 shift-jis로 처리해도 돌아가더라
        print(file_path)        
        content = content.decode('SHIFT_JIS').encode(to_encoding)

        # 변환된 내용을 다시 파일에 씁니다.
        with open(file_path, 'wb') as file:
            file.write(content)
        print(f'{file_path} converted from {current_encoding} to {to_encoding}')
    else:
        print(f'{file_path} is not in {from_encodings}')

# 지정된 디렉토리와 하위 디렉토리에 있는 모든 .cpp와 .h 파일을 찾아 인코딩을 변환합니다.
for filepath in Path(path).rglob('*'):
    if filepath.suffix in ['.cpp', '.h']:
        convert_encoding(filepath, ['SHIFT_JIS', 'Windows-1252'], 'utf-8')