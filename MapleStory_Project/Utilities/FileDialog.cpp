#include "stdafx.h"
#include "FileDialog.h"
#include <commdlg.h>

// Windows Open File Dialog를 열어 사용자가 선택한 파일 경로를 반환
std::wstring OpenFileDialog(const wchar_t* filter)
{
    wchar_t fileName[MAX_PATH] = {}; // 선택된 파일 경로를 저장할 버퍼

    OPENFILENAME openFileName = {}; // Windows 공용 파일 다이얼로그 설정 구조체
    openFileName.lStructSize = sizeof(OPENFILENAME); // 구조체 크기 설정

    // 표시할 파일 확장자 필터
    // 예 : "XML Files (*.xml)\0*.xml\0"
    openFileName.lpstrFilter = filter;

    openFileName.lpstrFile = fileName; // 사용자가 선택한 파일 경로가 저장될 버퍼
    openFileName.nMaxFile = MAX_PATH; // 파일 경로 버퍼의 최대 길이

    // 다이얼로그 동작 옵션
    // OFN_FILEMUSTEXIST : 존재하는 파일만 선택 가능
    // OFN_PATHMUSTEXIST : 존재하는 폴더 경로만 허용
    openFileName.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    openFileName.lpstrDefExt = L"xml"; // 기본 확장자 .xml 지정

    // 사용자가 파일을 선택하면 true 반환
    if (GetOpenFileName(&openFileName)) return fileName;

    // 취소 시 빈 문자열 반환
    return L"";
}

// Windows Save File Dialog를 열어 사용자가 저장할 파일 경로를 선택하도록 함
std::wstring SaveFileDialog(const wchar_t* filter)
{
    wchar_t fileName[MAX_PATH] = {}; // 선택된 파일 경로를 저장할 버퍼

    OPENFILENAME openFileName = {}; // Windows 공용 파일 다이얼로그 설정 구조체
    openFileName.lStructSize = sizeof(OPENFILENAME); // 구조체 크기 설정

    // 표시할 파일 확장자 필터
    // 예 : "XML Files (*.xml)\0*.xml\0"
    openFileName.lpstrFilter = filter;

    openFileName.lpstrFile = fileName; // 사용자가 입력한 저장 파일 경로가 저장될 버퍼
    openFileName.nMaxFile = MAX_PATH; // 파일 경로 버퍼 최대 길이

    // 다이얼로그 옵션
    // OFN_PATHMUSTEXIST : 존재하는 폴더만 선택 가능 (잘못된 경로에 저장하는 것을 방지)
    openFileName.Flags = OFN_PATHMUSTEXIST;

    openFileName.lpstrDefExt = L"xml"; // 기본 확장자 .xml 지정

    // 사용자가 저장할 파일을 선택하면 true 반환
    if (GetSaveFileName(&openFileName)) return fileName;

    // 취소 시 빈 문자열 반환
    return L"";
}
