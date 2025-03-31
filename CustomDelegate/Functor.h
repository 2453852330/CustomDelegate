#pragma once

// 封装任意的可调用对象（如 lambda / std::function / 函数指针）
// 实现 IDelegateInstance 的行为
// 将传入的函数或 lambda 存起来，并在调用时执行它

template<typename FuncType,typename UserPolicy, typename FunctorType>
class TBaseFunctorDelegateInstace : public UserPolicy::FDelegateInstanceExtras
{
public:
	// explicit 只能显示类型转换
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