#pragma once

#include <functional>
#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace qReal {

/// An entity that fires events with some value as parameter.
class AbstractListener
{
public:
	virtual ~AbstractListener() {}

	/// Fires event that some entity listens for with @arg value as parameter (if needed).
	virtual void fireEvent(QVariant const &value) = 0;
};

/// Useful class that calls the given slot with no parameters
template <typename Func>
class SlotListener0 : public AbstractListener
{
public:
	SlotListener0(typename QtPrivate::FunctionPointer<Func>::Object *sender, Func slot)
		: mSender(sender)
		, mSlot(slot)
	{
	}

	void fireEvent(QVariant const &value) override
	{
		Q_UNUSED(value)
		(mSender->*mSlot)();
	}

private:
	typename QtPrivate::FunctionPointer<Func>::Object *mSender;
	Func mSlot;
};

/// Useful class that calls the given lambda with no parameters.
class LambdaListener0 : public AbstractListener
{
public:
	LambdaListener0(std::function<void()> const & lambda)
		: mLambda(lambda)
	{
	}

	void fireEvent(QVariant const &value) override
	{
		Q_UNUSED(value)
		mLambda();
	}

private:
	std::function<void()> mLambda;
};

/// Useful class that calls the given slot with one parameter of the given type.
template <typename Type, typename Func>
class SlotListener1 : public AbstractListener
{
public:
	SlotListener1(typename QtPrivate::FunctionPointer<Func>::Object *sender, Func slot)
		: mSender(sender)
		, mSlot(slot)
	{
	}

	void fireEvent(QVariant const &value) override
	{
		(mSender->*mSlot)(value.value<Type>());
	}

private:
	typename QtPrivate::FunctionPointer<Func>::Object *mSender;
	Func mSlot;
};

/// Useful class that calls the given lambda with one parameter of the given type.
template <typename Type>
class LambdaListener1 : public AbstractListener
{
public:
	LambdaListener1(std::function<void(Type)> lambda)
		: mLambda(lambda)
	{
	}

	void fireEvent(QVariant const &value) override
	{
		mLambda(value.value<Type>());
	}

private:
	std::function<void(Type)> mLambda;
};

}
