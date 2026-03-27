#pragma once
#include "stdafx.h"
#include "BTNode.h"

//=========================================================================
// Selector Node (OR 연산 노드)
// 자식 노드들을 순서대로 실행하며 성공하거나 실행 중인 노드를 만나면 즉시 종료
//=========================================================================
class Selector : public BTNode
{
public:
	// 자식 노드 추가
	void AddChild(std::unique_ptr<BTNode> node)
	{
		children.push_back(move(node));
	}

	// Selector 실행
	BTState Tick(float deltaTime)
	{
		// 자식 노드를 순서대로 실행
		for (auto& child : children)
		{
			// 각 노드 Tick 수행
			BTState result = child->Tick(deltaTime);

			// Failure가 아니면 현재 행동을 선택하고 종료
			if (result != BTState::Failure)
				return result;
		}
		// 모든 자식이 실패했을 경우
		return BTState::Failure;
	}

private:
	// 실행 우선순서를 가지는 자식 노드 목록
	std::vector<std::unique_ptr<BTNode>> children;
};