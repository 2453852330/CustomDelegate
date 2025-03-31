#pragma once

// ��װ����Ŀɵ��ö����� lambda / std::function / ����ָ�룩
// ʵ�� IDelegateInstance ����Ϊ
// ������ĺ����� lambda �����������ڵ���ʱִ����

template<typename FuncType,typename UserPolicy, typename FunctorType>
class TBaseFunctorDelegateInstace : public UserPolicy::FDelegateInstanceExtras
{
public:
	// explicit ֻ����ʾ����ת��
	explicit TBaseFunctorDelegateInstace(FunctorType&& InFunc)
		:Functor(std::move(InFunc)){}

    virtual void Execute() override
    {
        Functor();
    }

    virtual bool IsBound() const override
    {
        return true;
    }

private:
	FunctorType Functor;
};