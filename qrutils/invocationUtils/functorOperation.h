#pragma once

#include <QtCore/QObject>
#include <QtCore/QThread>

#include "longOperation.h"
#include "private/functors.h"

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
///		void SomeClass::doOperation(int intArg, QString const &stringArg)
///		{
///		}
///		FunctorOperation<void> *operation = new FunctorOperation<void>(30000);
///		operation->setInvocationTarget(this, &SampleClass::doOperation, 5, true); // Class-member usage
///		connect(operation, SIGNAL(finished(invocation::InvocationResult)), this, SLOT(onOperationComplete(invocation::InvocationResult)));
///		operation->invoceAsync();
/// The second one  represents operation without progress providing.
/// For this got worked your function must accept Progress * type as
/// the first argument. Then you just call setInvocationTargetWithProgress(...)
/// with the function specifications and enumerate all args except the first one.
/// Args count must be less or equal 5 ('progress' arg + 4 args)
/// @code
///		void SomeClass::doOperation(Progress *progress, int intArg, QString const &stringArg)
///		{ // here you can use 'progress' parameter for progress providing
///		}
///		FunctorOperation<void> *operation = new FunctorOperation<void>(30000);
///		operation->setInvocationTargetWithProgress(this, &SampleClass::doOperation, 5, QString("some string"));
///		connect(operation, SIGNAL(finished(invocation::InvocationResult)), this, SLOT(onOperationComplete(invocation::InvocationResult)));
///		operation->invoceAsync();
/// @param TResult Must be function return value type
template<typename TResult>
class QRUTILS_EXPORT FunctorOperation : public LongOperation
{
public:
	/// @param timeout Operation timeout
	explicit FunctorOperation(int timeout = INT_MAX)
		: LongOperation(timeout)
	{
	}
	virtual ~FunctorOperation()
	{
	}

	// Too much overloads for having doc for each. See class definition doc
	template <typename T>
	void setInvocationTarget(T (*functionPointer)())
	{
		mFunctor = new StoredFunctorCall0<T, T (*)()>(functionPointer);
	}

	template <typename T, typename Param1, typename Arg1>
	void setInvocationTarget(T (*functionPointer)(Param1), const Arg1 &arg1)
	{
		mFunctor = new StoredFunctorCall1<T, T (*)(Param1), Arg1>(functionPointer, arg1);
	}

	template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTarget(T (*functionPointer)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
	{
		mFunctor = new StoredFunctorCall2<T, T (*)(Param1, Param2), Arg1, Arg2>(functionPointer, arg1, arg2);
	}

	template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTarget(T (*functionPointer)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		mFunctor = new StoredFunctorCall3<T, T (*)(Param1, Param2, Param3), Arg1, Arg2, Arg3>(functionPointer, arg1, arg2, arg3);
	}

	template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTarget(T (*functionPointer)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		mFunctor = new StoredFunctorCall4<T, T (*)(Param1, Param2, Param3, Param4), Arg1, Arg2, Arg3, Arg4>(functionPointer, arg1, arg2, arg3, arg4);
	}

	template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
	void setInvocationTarget(T (*functionPointer)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
	{
		mFunctor = new StoredFunctorCall5<T, T (*)(Param1, Param2, Param3, Param4, Param5), Arg1, Arg2, Arg3, Arg4, Arg5>(functionPointer, arg1, arg2, arg3, arg4, arg5);
	}

	template <typename FunctionObject>
	void setInvocationTarget(FunctionObject functionObject)
	{
		mFunctor = new StoredFunctorCall0<QT_TYPENAME FunctionObject::result_type, FunctionObject>(functionObject);
	}

	template <typename FunctionObject, typename Arg1>
	void setInvocationTarget(FunctionObject functionObject, const Arg1 &arg1)
	{
		mFunctor = new StoredFunctorCall1<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1>(functionObject, arg1);
	}

	template <typename FunctionObject, typename Arg1, typename Arg2>
	void setInvocationTarget(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2)
	{
		mFunctor = new StoredFunctorCall2<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2>(functionObject, arg1, arg2);
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3>
	void setInvocationTarget(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		mFunctor = new StoredFunctorCall3<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3>(functionObject, arg1, arg2, arg3);
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void setInvocationTarget(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		mFunctor = new StoredFunctorCall4<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3, Arg4>(functionObject, arg1, arg2, arg3, arg4);
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	void setInvocationTarget(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
	{
		mFunctor = new StoredFunctorCall5<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3, Arg4, Arg5>(functionObject, arg1, arg2, arg3, arg4, arg5);
	}

	template <typename FunctionObject>
	void setInvocationTarget(FunctionObject *functionObject)
	{
		mFunctor = new QT_TYPENAME SelectStoredFunctorPointerCall0<QT_TYPENAME FunctionObject::result_type, FunctionObject>::type(functionObject);
	}

	template <typename FunctionObject, typename Arg1>
	void setInvocationTarget(FunctionObject *functionObject, const Arg1 &arg1)
	{
		mFunctor = new QT_TYPENAME SelectStoredFunctorPointerCall1<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1>::type(functionObject, arg1);
	}

	template <typename FunctionObject, typename Arg1, typename Arg2>
	void setInvocationTarget(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2)
	{
		mFunctor = new QT_TYPENAME SelectStoredFunctorPointerCall2<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2>::type(functionObject, arg1, arg2);
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3>
	void setInvocationTarget(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		mFunctor = new QT_TYPENAME SelectStoredFunctorPointerCall3<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3>::type(functionObject, arg1, arg2, arg3);
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void setInvocationTarget(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		mFunctor = new QT_TYPENAME SelectStoredFunctorPointerCall4<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3, Arg4>::type(functionObject, arg1, arg2, arg3, arg4);
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	void setInvocationTarget(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
	{
		mFunctor = new QT_TYPENAME SelectStoredFunctorPointerCall5<QT_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3, Arg4, Arg5>::type(functionObject, arg1, arg2, arg3, arg4, arg5);
	}

	template <typename T, typename Class>
	void setInvocationTarget(const Class &object, T (Class::*fn)())
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionCall0<T, Class>::type(fn, object);
	}

	template <typename T, typename Class, typename Param1, typename Arg1>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1), const Arg1 &arg1)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionCall1<T, Class, Param1, Arg1>::type(fn, object, arg1);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionCall2<T, Class, Param1, Arg1, Param2, Arg2>::type(fn, object, arg1, arg2);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>::type(fn, object, arg1, arg2, arg3);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>::type(fn, object, arg1, arg2, arg3, arg4);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>::type(fn, object, arg1, arg2, arg3, arg4, arg5);
	}

	template <typename T, typename Class>
	void setInvocationTarget(const Class &object, T (Class::*fn)() const)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionCall0<T, Class>::type(fn, object);
	}

	template <typename T, typename Class, typename Param1, typename Arg1>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1) const, const Arg1 &arg1)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionCall1<T, Class, Param1, Arg1>::type(fn, object, arg1);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1, Param2) const, const Arg1 &arg1, const Arg2 &arg2)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionCall2<T, Class, Param1, Arg1, Param2, Arg2>::type(fn, object, arg1, arg2);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1, Param2, Param3) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>::type(fn, object, arg1, arg2, arg3);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>::type(fn, object, arg1, arg2, arg3, arg4);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
	void setInvocationTarget(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>::type(fn, object, arg1, arg2, arg3, arg4, arg5);
	}

	template <typename T, typename Class>
	void setInvocationTarget(Class *object, T (Class::*fn)())
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionPointerCall0<T, Class>::type(fn, object);
	}

	template <typename T, typename Class, typename Param1, typename Arg1>
	void setInvocationTarget(Class *object, T (Class::*fn)(Param1), const Arg1 &arg1)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionPointerCall1<T, Class, Param1, Arg1>::type(fn, object, arg1);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTarget(Class *object, T (Class::*fn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionPointerCall2<T, Class, Param1, Arg1, Param2, Arg2>::type(fn, object, arg1, arg2);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTarget(Class *object, T (Class::*fn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionPointerCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>::type(fn, object, arg1, arg2, arg3);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTarget(Class *object, T (Class::*fn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionPointerCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>::type(fn, object, arg1, arg2, arg3, arg4);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
	void setInvocationTarget(Class *object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
	{
		mFunctor = new QT_TYPENAME SelectStoredMemberFunctionPointerCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>::type(fn, object, arg1, arg2, arg3, arg4, arg5);
	}

	template <typename T, typename Class>
	void setInvocationTarget(const Class *object, T (Class::*fn)() const)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionPointerCall0<T, Class>::type(fn, object);
	}

	template <typename T, typename Class, typename Param1, typename Arg1>
	void setInvocationTarget(const Class *object, T (Class::*fn)(Param1) const, const Arg1 &arg1)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionPointerCall1<T, Class, Param1, Arg1>::type(fn, object, arg1);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTarget(const Class *object, T (Class::*fn)(Param1, Param2) const, const Arg1 &arg1, const Arg2 &arg2)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionPointerCall2<T, Class, Param1, Arg1, Param2, Arg2>::type(fn, object, arg1, arg2);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTarget(const Class *object, T (Class::*fn)(Param1, Param2, Param3) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionPointerCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>::type(fn, object, arg1, arg2, arg3);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTarget(const Class *object, T (Class::*fn)(Param1, Param2, Param3, Param4) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionPointerCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>::type(fn, object, arg1, arg2, arg3, arg4);
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
	void setInvocationTarget(const Class *object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
	{
		mFunctor = new QT_TYPENAME SelectStoredConstMemberFunctionPointerCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>::type(fn, object, arg1, arg2, arg3, arg4, arg5);
	}


	template <typename T>
	void setInvocationTargetWithProgress(T (*functionPointer)(Progress *))
	{
		setInvocationTarget(functionPointer, mProgress);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Param1, typename Arg1>
	void setInvocationTargetWithProgress(T (*functionPointer)(Progress *, Param1), const Arg1 &arg1)
	{
		setInvocationTarget(functionPointer, mProgress, arg1);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTargetWithProgress(T (*functionPointer)(Progress *, Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
	{
		setInvocationTarget(functionPointer, mProgress, arg1, arg2);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTargetWithProgress(T (*functionPointer)(Progress *, Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		setInvocationTarget(functionPointer, mProgress, arg1, arg2, arg3);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTargetWithProgress(T (*functionPointer)(Progress *, Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		setInvocationTarget(functionPointer, mProgress, arg1, arg2, arg3, arg4);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject>
	void setInvocationTargetWithProgress(FunctionObject functionObject)
	{
		setInvocationTarget(functionObject, mProgress);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject, typename Arg1>
	void setInvocationTargetWithProgress(FunctionObject functionObject, const Arg1 &arg1)
	{
		setInvocationTarget(functionObject, mProgress, arg1);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject, typename Arg1, typename Arg2>
	void setInvocationTargetWithProgress(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2)
	{
		setInvocationTarget(functionObject, mProgress, arg1, arg2);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3>
	void setInvocationTargetWithProgress(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		setInvocationTarget(functionObject, mProgress, arg1, arg2, arg3);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void setInvocationTargetWithProgress(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		setInvocationTarget(functionObject, mProgress, arg1, arg2, arg3, arg4);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject>
	void setInvocationTargetWithProgress(FunctionObject *functionObject)
	{
		setInvocationTarget(functionObject, mProgress);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject, typename Arg1>
	void setInvocationTargetWithProgress(FunctionObject *functionObject, const Arg1 &arg1)
	{
		setInvocationTarget(functionObject, mProgress, arg1);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject, typename Arg1, typename Arg2>
	void setInvocationTargetWithProgress(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2)
	{
		setInvocationTarget(functionObject, mProgress, arg1, arg2);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3>
	void setInvocationTargetWithProgress(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		setInvocationTarget(functionObject, mProgress, arg1, arg2, arg3);
		mIsOperationWithProgress = true;
	}

	template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void setInvocationTargetWithProgress(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		setInvocationTarget(functionObject, mProgress, arg1, arg2, arg3, arg4);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)())
	{
		setInvocationTarget(object, fn, mProgress);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)(Param1), const Arg1 &arg1)
	{
		setInvocationTarget(object, fn, mProgress, arg1);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2, arg3);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2, arg3, arg4);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)() const)
	{
		setInvocationTarget(object, fn, mProgress);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)(Param1) const, const Arg1 &arg1)
	{
		setInvocationTarget(object, fn, mProgress, arg1);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)(Param1, Param2) const, const Arg1 &arg1, const Arg2 &arg2)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)(Param1, Param2, Param3) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2, arg3);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTargetWithProgress(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2, arg3, arg4);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class>
	void setInvocationTargetWithProgress(Class *object, T (Class::*fn)(Progress *))
	{
		setInvocationTarget(object, fn, mProgress);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1>
	void setInvocationTargetWithProgress(Class *object, T (Class::*fn)(Progress *, Param1), const Arg1 &arg1)
	{
		setInvocationTarget(object, fn, mProgress, arg1);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTargetWithProgress(Class *object, T (Class::*fn)(Progress *, Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTargetWithProgress(Class *object, T (Class::*fn)(Progress *, Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2, arg3);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTargetWithProgress(Class *object, T (Class::*fn)(Progress *, Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2, arg3, arg4);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class>
	void setInvocationTargetWithProgress(const Class *object, T (Class::*fn)(Progress *) const)
	{
		setInvocationTarget(object, fn, mProgress);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1>
	void setInvocationTargetWithProgress(const Class *object, T (Class::*fn)(Progress *, Param1) const, const Arg1 &arg1)
	{
		setInvocationTarget(object, fn, mProgress, arg1);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
	void setInvocationTargetWithProgress(const Class *object, T (Class::*fn)(Progress *, Param1, Param2) const, const Arg1 &arg1, const Arg2 &arg2)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
	void setInvocationTargetWithProgress(const Class *object, T (Class::*fn)(Progress *, Param1, Param2, Param3) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2, arg3);
		mIsOperationWithProgress = true;
	}

	template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
	void setInvocationTargetWithProgress(const Class *object, T (Class::*fn)(Progress *, Param1, Param2, Param3, Param4) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
	{
		setInvocationTarget(object, fn, mProgress, arg1, arg2, arg3, arg4);
		mIsOperationWithProgress = true;
	}

	/// Returns value returned by specified function.
	/// In case when function returns nothing (void)
	/// the behaviour is unknown
	TResult *result() const
	{
		return mResult;
	}

signals:
	void beforeStarted();
	void finished(InvocationState invocationResult);

private:
	virtual void onThreadFinished()
	{
		mResult = (dynamic_cast<FunctorThread *>(mThread))->result();
		LongOperation::onThreadFinished();
	}

	virtual void startInvocation(QThread::Priority priority = QThread::NormalPriority)
	{
		mThread = new FunctorThread(mFunctor);
		LongOperation::startInvocation(priority);
	}

	class FunctorThread : public QThread
	{
	public:
		FunctorThread(Functor<TResult> *functor)
			: mFunctor(functor)
		{
		}

		TResult *result() const
		{
			return mResult;
		}

		void run()
		{
			mFunctor->runFunctor();
		}

	private:
		Functor<TResult> *mFunctor;
		TResult *mResult;
	};

	Functor<TResult> *mFunctor;
	TResult *mResult;
};

}
