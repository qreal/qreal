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

#include <QtCore/QObject>
#include <QtCore/QThread>

#include "private/functors.h"
#include "longOperation.h"


using namespace invocation::details;

#define QT_TYPENAME typename

namespace invocation
{

/// @brief Represents long operation derived from some function or class member
/// Usage: there are two usage scenarious.
/// The first scenario represents operation without progress providing.
/// You just call setInvocationTarget(...) with the function specification
/// and enumerate arguments. Args count must be less or equal 5
/// @code
///		void SomeClass::doOperation(int intArg, const QString &stringArg)
///		{
///		}
///		FunctorOperation<void> *operation = new FunctorOperation<void>(30000);
///		operation->setInvocationTarget(this, &SampleClass::doOperation, 5, true); // Class-member usage
///		connect(operation, SIGNAL(finished(invocation::InvocationResult))
///				, this, SLOT(onOperationComplete(invocation::InvocationResult)));
///		operation->invoceAsync();
/// The second one  represents operation without progress providing.
/// For this got worked your function must accept Progress * type as
/// the first argument. Then you just call setInvocationTargetWithProgress(...)
/// with the function specifications and enumerate all args except the first one.
/// Args count must be less or equal 5 ('progress' arg + 4 args)
/// @code
///		void SomeClass::doOperation(Progress *progress, int intArg, const QString &stringArg)
///		{ // here you can use 'progress' parameter for progress providing
///		}
///		FunctorOperation<void> *operation = new FunctorOperation<void>(30000);
///		operation->setInvocationTargetWithProgress(this, &SampleClass::doOperation, 5, QString("some string"));
///		connect(operation, SIGNAL(finished(invocation::InvocationResult))
///				, this, SLOT(onOperationComplete(invocation::InvocationResult)));
///		operation->invoceAsync();
///
/// @param TResult Must be function return value type

template<typename TResult>
class FunctorOperation : public LongOperation
{
public:
	/// @param timeout Operation timeout
	explicit FunctorOperation(details::FunctorInterface<TResult> *functorInterface, int timeout = INT_MAX)
		: LongOperation(timeout), mFunctor(functorInterface)
	{
		if (mFunctor->withProgress()) {
			mFunctor->setProgressIfNecessary(mProgress);
		}
	}

	virtual ~FunctorOperation()
	{
	}

	/// Returns value returned by specified function.
	/// In case when function returns nothing (void)
	/// the behaviour is unknown
	TResult result() const
	{
		if (mFunctor->isReturnSmth()) {
			return mFunctor->result();
		}
	}

signals:
	void beforeStarted();
	void finished(InvocationState invocationResult);

private:
	virtual void startInvocation(QThread::Priority priority = QThread::NormalPriority)
	{
		mThread = new FunctorThread(mFunctor);
		LongOperation::startInvocation(priority);
	}

	class FunctorThread : public QThread
	{
	public:
		FunctorThread(details::FunctorInterface<TResult> *functor)
			: mFunctor(functor)
		{
		}

		void run()
		{
			mFunctor->startFunc();
		}

	private:
		details::FunctorInterface<TResult> *mFunctor;
	};

	details::FunctorInterface<TResult> *mFunctor;
};

}
