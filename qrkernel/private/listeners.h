/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	virtual void fireEvent(const QVariant &value) = 0;

	/// Returns the object that can be used for disconnection;
	/// if no such object was provided explicitly for this listener nullptr is returned.
	virtual const QObject *object() const = 0;
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

	void fireEvent(const QVariant &value) override
	{
		Q_UNUSED(value)
		(mSender->*mSlot)();
	}

	const QObject *object() const override
	{
		return mSender;
	}

private:
	typename QtPrivate::FunctionPointer<Func>::Object *mSender;
	Func mSlot;
};

/// Useful class that calls the given lambda with no parameters.
class LambdaListener0 : public AbstractListener
{
public:
	LambdaListener0(std::function<void()> const & lambda, QObject *owner = 0)
		: mLambda(lambda)
		, mOwner(owner)
	{
	}

	void fireEvent(const QVariant &value) override
	{
		Q_UNUSED(value)
		mLambda();
	}

	const QObject *object() const override
	{
		return mOwner;
	}

private:
	std::function<void()> mLambda;
	QObject *mOwner;
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

	void fireEvent(const QVariant &value) override
	{
		(mSender->*mSlot)(value.value<Type>());
	}

	const QObject *object() const override
	{
		return mSender;
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
	LambdaListener1(std::function<void(Type)> lambda, QObject *owner = 0)
		: mLambda(lambda)
		, mOwner(owner)
	{
	}

	void fireEvent(const QVariant &value) override
	{
		mLambda(value.value<Type>());
	}

	const QObject *object() const override
	{
		return mOwner;
	}

private:
	std::function<void(Type)> mLambda;
	QObject *mOwner;
};

}
