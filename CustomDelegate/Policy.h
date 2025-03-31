#pragma once
#include "IDelegateInstance.h"

// 提供策略扩展点，可以做很多灵活处理：
struct FDefaultDelegateUserPolicy {

    struct FDelegateExtras {}; // 暂无扩展

    struct FDelegateInstanceExtras : public IDelegateInstance
    {
        // 可以用于统计、调试、线程保护等
    };

};

