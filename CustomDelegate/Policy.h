#pragma once
#include "IDelegateInstance.h"

// �ṩ������չ�㣬�������ܶ�����
struct FDefaultDelegateUserPolicy {

    struct FDelegateExtras {}; // ������չ

    struct FDelegateInstanceExtras : public IDelegateInstance
    {
        // ��������ͳ�ơ����ԡ��̱߳�����
    };

};

