#include "stdafx.h"
#include "Monster.h"

// Monster 생성자(기본 텍스처 경로와 이름을 설정)
Monster::Monster(const std::wstring& texturePath, BodyType bodyType, const std::string& name)
	: texturePath(texturePath), name(name)
{
}

// Monster 소멸자
Monster::~Monster()
{
}
