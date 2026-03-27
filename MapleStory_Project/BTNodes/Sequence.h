#pragma once
#include "stdafx.h"
#include "BTNode.h"

//========================================================
// Sequence Node (AND 연산 노드)
// 자식 노드를 순서대로 실행하며 하나라도 실패하면 즉시 종료
// 모든 자식이 Success여야 Success 반환
//========================================================
class Sequence : public BTNode
{
public:
    // 자식 노드 추가
    void AddChild(std::unique_ptr<BTNode> node)
    {
        children.push_back(std::move(node));
    }

    // Sequence 실행
    BTState Tick(float deltaTime) override
    {
        // 자식 노드 순차 실행
        for (auto& child : children)
        {
            BTState result = child->Tick(deltaTime);

            // Success가 아니면 즉시 종료
            if (result != BTState::Success)
                return result;
        }

        // 모든 자식이 성공했을 경우
        return BTState::Success;
    }

private:
    // 순차적으로 실행될 자식 노드 리스트
    std::vector<std::unique_ptr<BTNode>> children;
};