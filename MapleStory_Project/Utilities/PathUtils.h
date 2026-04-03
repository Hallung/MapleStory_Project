#pragma once
#include <filesystem>

//====================================================
// 실행 파일 기준으로 프로젝트 루트 경로를 찾아
// 상대 경로를 절대 경로로 변환해주는 유틸리티 클래스
// (Static 방식으로 전역에서 사용)
//====================================================

class PathUtils
{
public:
    // 실행 파일 위치를 기준으로 assetRoot 경로 설정
    static void Init();

    // 상대 경로를 std::filesystem::path로 변환
    static std::filesystem::path GetFullPath(const std::wstring& relativePath);
    static std::filesystem::path GetFullPath(const std::string& relativePath);

    // 상대 경로를 wstring으로 변환
    static std::wstring GetFullPathW(const std::wstring& relativePath);
    // 상대 경로를 string으로 변환
    static std::string GetFullPathS(const std::string& relativePath);

private:
    // 프로젝트 루트 경로 (Init에서 한 번 설정되며 이후 변경되지 않음)
    static std::filesystem::path assetRoot;
};