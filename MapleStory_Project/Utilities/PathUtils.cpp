#include "stdafx.h"
#include "PathUtils.h"

// static 멤버 변수 정의 (실제 메모리 할당)
std::filesystem::path PathUtils::assetRoot;

// 실행 파일 위치를 기준으로 프로젝트 루트 경로 설정
void PathUtils::Init()
{
    wchar_t buffer[MAX_PATH];

    // 현재 실행 파일 경로 가져오기
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);

    // exe 파일이 있는 폴더 경로
    std::filesystem::path exePath = std::filesystem::path(buffer).parent_path();

    // 빌드 폴더 구조에 따라 상위 폴더로 이동 (Debug / Release / x64 / Build 등 제거)
    if (exePath.filename() == L"Debug" || exePath.filename() == L"Release")
        exePath = exePath.parent_path();

    if (exePath.filename() == L"x64")
        exePath = exePath.parent_path();

    if (exePath.filename() == L"Build")
        exePath = exePath.parent_path();

    // 프로젝트 루트 경로 구성
    std::filesystem::path projectPath = exePath / L"MapleStory_Project";

    // 해당 경로가 존재하면 assetRoot로 설정
    if (std::filesystem::exists(projectPath) && std::filesystem::is_directory(projectPath))
    {
        assetRoot = projectPath.wstring();
    }
    // 실패 시 exe 경로를 기본 경로로 사용
    else
    {
        assetRoot = exePath.wstring();
    }

#ifdef _DEBUG
    // 디버그 환경에서는 경로가 실제 존재하는지 검증
    assert(std::filesystem::exists(assetRoot));
#endif
}

// wstring 상대 경로 -> path 변환
std::filesystem::path PathUtils::GetFullPath(const std::wstring& relativePath)
{
    return GetFullPathW(relativePath);
}

// string 상대 경로 -> path 변환
std::filesystem::path PathUtils::GetFullPath(const std::string& relativePath)
{
    return GetFullPathS(relativePath);
}

// wstring 상대 경로 -> 절대 경로(wstring)
std::wstring PathUtils::GetFullPathW(const std::wstring& relativePath)
{
    std::filesystem::path base(assetRoot);
    // base 경로 + 상대 경로 결합
    std::filesystem::path full = base / relativePath;

    return full.wstring();
}

// string 상대 경로 -> 절대 경로(string)
std::string PathUtils::GetFullPathS(const std::string& relativePath)
{
    std::filesystem::path base(assetRoot);
    // base 경로 + 상대 경로 결합
    std::filesystem::path full = base / relativePath;

    return full.string();
}
