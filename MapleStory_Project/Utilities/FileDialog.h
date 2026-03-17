#pragma once
// Windows 파일 탐색기 Dialog를 열기 위한 유틸리티 함수
// TileMap Save / Load 시 사용자가 직접 경로를 선택할 수 있도록 제공

// Windows 파일 열기 다이얼로그
std::wstring OpenFileDialog(const wchar_t* filter);

// Windows 파일 저장 다이얼로그
std::wstring SaveFileDialog(const wchar_t* filter);