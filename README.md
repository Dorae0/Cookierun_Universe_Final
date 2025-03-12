# Cookierun : Universe

<br>

![image](https://github.com/user-attachments/assets/de5d859c-7d4c-48b3-b896-d3967a6d4870)

<br>

## 📝 [목차](#index) <a name = "index"></a>

### $\color{#cc0000}\ 각\ 항목의\ 이모지를\ 누르면\ 목차로\ 돌아올\ 수\ 있습니다!\ $

- [🎨 스크린샷](#screenshots)
- [🚩 게임 컨셉](#concept)
- [🎮 다운로드 방법](#download)
- [🖥️ 기술 스택](#stacks)
- [⚙️ 프로젝트 아키텍처](#architecture) 
- [🧑‍💻 주요 코드](#code)

<br>

## [🎨](#index) 스크린샷 <a name = "screenshots"></a>

![image](https://github.com/user-attachments/assets/6d444c66-b9e5-434f-8aad-36c597a72f3b)
<br>

![image](https://github.com/user-attachments/assets/ac16c922-393d-4bf7-8854-68cd05c7ad76)
<br>

![image](https://github.com/user-attachments/assets/84550b84-8b82-4c65-97a1-bbc1c377e050)
<br>

![image](https://github.com/user-attachments/assets/952b91cf-86f8-4e12-93f2-1ce174abad1c)
<br>

![image](https://github.com/user-attachments/assets/24ae615c-dcd5-4c93-a96a-9c22cba9d6be)
<br>

![image](https://github.com/user-attachments/assets/380f3bec-ba75-40b7-af51-fcbbdcfba7ec)
<br><br>

## [🚩](#index) 게임 컨셉 <a name = "concept"></a>

- 간단한 디펜스 게임입니다.
- 쿠키런 IP를 차용해서 제작했습니다.
- Windows API를 사용하여 제작했습니다.

<br>

## [🎮](#index) 다운로드 방법 <a name = "download"></a>

### 1. branch를 download로 변경
![image](https://github.com/user-attachments/assets/1827760d-8eb3-493d-bba8-8ab3d8488606)
<br><br><br>

### 2. Code > Download ZIP 클릭
![image](https://github.com/user-attachments/assets/1655ada0-40a9-49ab-af6f-da77cd5ef3a1)
<br><br><br>

### 3. 압축 해제 후 Cookierun Universe 실행
![image](https://github.com/user-attachments/assets/4dbd221b-d927-4c4e-a068-6af494c69eaf)
<br><br><br>

### 3 - 1. Windows PC 보호가 뜬다면?
![image](https://github.com/user-attachments/assets/fab9402c-a56d-45da-aba0-a6fc256d1225)
<br><br><br>

### 3 - 2. "추가 정보" 클릭
![제목 없음](https://github.com/user-attachments/assets/40f1aefd-f379-42e6-b1c7-ece20842f2f0)
<br><br><br>

### 3 - 3. "실행" 클릭
![image](https://github.com/user-attachments/assets/0c6175ec-d489-472d-abf3-23bef71bb22a)
<br><br><br>

## [🖥️](#index) 기술 스택 <a name = "stacks"></a>

#### Common stacks
<img alt="Git" src ="https://img.shields.io/badge/Git-F05032.svg?&style=for-the-badge&logo=Git&logoColor=white"> <img alt="GitHub" src ="https://img.shields.io/badge/GitHub-181717.svg?&style=for-the-badge&logo=Github&logoColor=white">

#### IDE
<img alt="Visual studio" src ="https://img.shields.io/badge/Visual studio-7252aa.svg?&style=for-the-badge&logo=1&logoColor=white"/>

#### Languages
<img alt="CPlusPlus" src ="https://img.shields.io/badge/C++-00599C.svg?&style=for-the-badge&logo=CPlusPlus&logoColor=white"> <img alt="WindowsAPI" src ="https://img.shields.io/badge/Windows API-00599C.svg?&style=for-the-badge&logo=CPlusPlus&logoColor=white">
<br>

## [⚙️](#index) 프로젝트 아키텍처 <a name = "architecture"></a>

```cpp
프로젝트 루트
 ├── 소스 파일
 │   ├── Cookies                     // 쿠키 캐릭터 관련 클래스
 │   │   ├── CBraveCookie.cpp        // 용감한 쿠키
 │   │   ├── CCookie.cpp             // 쿠키 전체 클래스
 │   │   ├── CCottoncandyCookie.cpp  // 솜사탕맛 쿠키
 │   │   ├── CPistachioCookie.cpp    // 피스타치오맛 쿠키
 │   │   ├── CPitayaCookie.cpp       // 용과맛 쿠키
 │   │
 │   ├── Mobs                        // 몹 관련 클래스
 │   │   ├── CMob.cpp                // 몹 전체 클래스
 │   │   ├── CYellowBee.cpp          // 노란색 벌
 │   │
 │   ├── CookieMove.cpp              // 쿠키 이동 관련 로직
 │   ├── DoubleBuffering.cpp         // 더블 버퍼링 관련 로직 (이미지 로드 및 출력 관련)
 │   ├── main.cpp                    // 프로그램 진입점
 │
 ├── 헤더 파일
 │   ├── Cookies                     // 쿠키 캐릭터 관련 헤더
 │   │   ├── CBraveCookie.h          // 용감한 쿠키
 │   │   ├── CCookie.h               // 쿠키 전체 헤더
 │   │   ├── CCottoncandyCookie.h    // 솜사탕맛 쿠키
 │   │   ├── CPistachioCookie.h      // 피스타치오맛 쿠키
 │   │   ├── CPitayaCookie.h         // 용과맛 쿠키
 │   │
 │   ├── Mobs                        // 몹 관련 헤더
 │   │   ├── CMob.h                  // 몹 전체 헤더
 │   │   ├── CYellowBee.h            // 노란색 벌
 │   │
 │   ├── CookieMove.h                // 쿠키 이동 관련 헤더
 │   ├── DoubleBuffering.h           // 더블 버퍼링 관련 헤더 (이미지 로드 및 출력 관련)
```
 
## [🧑‍💻](#index) 주요 코드 <a name = "code"></a>
