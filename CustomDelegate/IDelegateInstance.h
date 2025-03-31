#pragma once

// 
class IDelegateInstance {
public:

	virtual void Execute() = 0;
	virtual bool IsBound() const = 0;
	virtual ~IDelegateInstance() = default;
};

