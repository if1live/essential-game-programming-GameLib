# essential game programming

![게임 프로그래밍의 정석](https://image.yes24.com/momo/TopCate166/MidCate06/16552657.jpg)

https://www.yes24.com/Product/Goods/6230931

* 제목: 세가의 신입 사원 교육 과정에서 배우는 게임 프로그래밍의 정석
* 부제: C++ 2D 콘솔 게임 개발에서 DirectX로 3D 게임 개발까지 한 번에 배운다
* 저자: 히라야마 타카시

https://download.hanbit.co.kr/exam/1899/
제공된 vs2010 예제를 기반으로 일부를 수정했다.

## 변경 사항

* h, cpp 인코딩을 SHIFT-JIS 에서 UTF-8로 변경
	* convert_encoding.py
* UTF-8로 인코딩 변경후에 발생한 컴파일 에러 땜질
	* 일본어 문자열을 영어로 바꿈
* vcxproj에 `<PlatformToolset>v143</PlatformToolset>` 추가
	* convert_vcxproj.py
	
## 실행 절차

git clone된 저장소의 경로를 `GAME_LIB_DIR` 환경변수로 등록하자.
환경변수를 설정하지 않을 경우, 솔루션을 고쳐야하는데 고쳐야할 범위가 너무 넓다.

`src/GameLibs/Modules/Modules.sln`을 빌드해야 한다.
모든 예제에서 공통적으로 쓰인다. 한번만 빌드해도 된다.

`src/02_2DGraphics1/2DGraphics1.sln`을 빌드하려고 한다고 치자.
프로젝트 -> 속성 -> C/C++ -> 일반 -> 추가 포함 디렉토리를 확인한다.
여기에 연결된 프로젝트를 먼저 빌드해야된다.
이번 시나리오에서는 `$(GAME_LIB_DIR)\2DGraphics1\include`가 들어있다.

`src/GameLibs`에서 이에 대응되는 항목을 먼저 빌드해야된다.
`src/GameLibs/2DGraphics1/GameLib.sln`을 빌드한다.
