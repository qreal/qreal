#pragma once

#include <QtCore/qglobal.h>
#include <QtConcurrent/qtconcurrentcompilertest.h>

namespace invocation
{
namespace details
{

template<typename T>
class Functor
{
public:
	virtual void runFunctor() = 0;

	T result() const
	{
		return mResult;
	}

protected:
	T mResult;
};

template <>
class Functor<void>
{
public:
	virtual void runFunctor() = 0;
};

template <typename T>
struct SelectSpecialization
{
	template <class Normal, class Void>
	struct Type { typedef Normal type; };
};

template <>
struct SelectSpecialization<void>
{
	template <class Normal, class Void>
	struct Type { typedef Void type; };
};

template <typename T, typename FunctionPointer>
struct StoredFunctorCall0: public Functor<T>
{
	inline StoredFunctorCall0(FunctionPointer function)
	  : mFunction(function)
	{
	}

	void runFunctor()
	{
		this->mResult = mFunction();
	}

	FunctionPointer mFunction;
};

template <typename FunctionPointer>
struct StoredFunctorCall0<void, FunctionPointer>: public Functor<void>
{
	inline StoredFunctorCall0(FunctionPointer function)
	  : mFunction(function)
	{
	}

	void runFunctor()
	{
		mFunction();
	}

	FunctionPointer mFunction;
};

template <typename T, typename FunctionPointer>
struct StoredFunctorPointerCall0: public Functor<T>
{
	inline StoredFunctorPointerCall0(FunctionPointer *function)
	  : mFunction(function)
	{
	}

	void runFunctor()
	{
		this->mResult =(*mFunction)();
	}

	FunctionPointer *mFunction;
};

template <typename T, typename FunctionPointer>
struct VoidStoredFunctorPointerCall0: public Functor<T>
{
	inline VoidStoredFunctorPointerCall0(FunctionPointer *function)
	  : mFunction(function)
	{
	}

	void runFunctor()
	{
		(*mFunction)();
	}

	FunctionPointer *mFunction;
};

template <typename T, typename FunctionPointer>
struct SelectStoredFunctorPointerCall0
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredFunctorPointerCall0    <T, FunctionPointer>,
			 VoidStoredFunctorPointerCall0<T, FunctionPointer> >::type type;
};

template <typename T, typename Class>
class StoredMemberFunctionCall0 : public Functor<T>
{
public:
	StoredMemberFunctionCall0(T (Class::*fn)() , Class const &object)
	  : mFn(fn), mObject(object)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)();
	}

private:
	T (Class::*mFn)();
	Class mObject;
};

template <typename T, typename Class>
class VoidStoredMemberFunctionCall0 : public Functor<T>
{
public:
	VoidStoredMemberFunctionCall0(T (Class::*_fn)() , Class const &_object)
	: fn(_fn), object(_object){ }

	void runFunctor()
	{
		(object.*fn)();
	}
private:
	T (Class::*fn)();
	Class object;

};
template <typename T, typename Class>
struct SelectStoredMemberFunctionCall0
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionCall0    <T, Class>,
			 VoidStoredMemberFunctionCall0<T, Class> >::type type;
};
template <typename T, typename Class>
class StoredConstMemberFunctionCall0 : public Functor<T>
{
public:
	StoredConstMemberFunctionCall0(T (Class::*_fn)() const, Class const &_object)
	: fn(_fn), object(_object){ }

	void runFunctor()
	{
		this->mResult = (object.*fn)();
	}
private:
	T (Class::*fn)()const;
	Class const object;

};
template <typename T, typename Class>
class VoidStoredConstMemberFunctionCall0 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionCall0(T (Class::*_fn)() const, Class const &_object)
	: fn(_fn), object(_object){ }

	void runFunctor()
	{
		(object.*fn)();
	}
private:
	T (Class::*fn)()const;
	Class const object;

};
template <typename T, typename Class>
struct SelectStoredConstMemberFunctionCall0
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionCall0    <T, Class>,
			 VoidStoredConstMemberFunctionCall0<T, Class> >::type type;
};
template <typename T, typename Class>
class StoredMemberFunctionPointerCall0 : public Functor<T>
{
public:
	StoredMemberFunctionPointerCall0(T (Class::*_fn)() , Class *_object)
	: fn(_fn), object(_object){ }

	void runFunctor()
	{
		this->mResult = (object->*fn)();
	}
private:
	T (Class::*fn)();
	Class *object;

};
template <typename T, typename Class>
class VoidStoredMemberFunctionPointerCall0 : public Functor<T>
{
public:
	VoidStoredMemberFunctionPointerCall0(T (Class::*_fn)() , Class *_object)
	: fn(_fn), object(_object){ }

	void runFunctor()
	{
		(object->*fn)();
	}
private:
	T (Class::*fn)();
	Class *object;

};
template <typename T, typename Class>
struct SelectStoredMemberFunctionPointerCall0
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionPointerCall0    <T, Class>,
			 VoidStoredMemberFunctionPointerCall0<T, Class> >::type type;
};
template <typename T, typename Class>
class StoredConstMemberFunctionPointerCall0 : public Functor<T>
{
public:
	StoredConstMemberFunctionPointerCall0(T (Class::*_fn)() const, Class const *_object)
	: fn(_fn), object(_object){ }

	void runFunctor()
	{
		this->mResult = (object->*fn)();
	}
private:
	T (Class::*fn)()const;
	Class const *object;

};

template <typename T, typename Class>
class VoidStoredConstMemberFunctionPointerCall0 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionPointerCall0(T (Class::*fn)() const, Class const *object)
	  : mFn(fn), mObject(object)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)();
	}

private:
	T (Class::*mFn)() const;
	Class const *mObject;
};

template <typename T, typename Class>
struct SelectStoredConstMemberFunctionPointerCall0
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionPointerCall0    <T, Class>,
			 VoidStoredConstMemberFunctionPointerCall0<T, Class> >::type type;
};

template <typename T, typename FunctionPointer, typename Arg1>
struct StoredFunctorCall1 : public Functor<T>
{
	inline StoredFunctorCall1(FunctionPointer function, Arg1 const &arg1)
	  : mFunction(function), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		this->mResult = function(mArg1);
	}

	FunctionPointer mFunction;
	Arg1 mArg1;
};

template <typename FunctionPointer, typename Arg1>
struct StoredFunctorCall1<void, FunctionPointer, Arg1> : public Functor<void>
{
	inline StoredFunctorCall1(FunctionPointer function, Arg1 const &arg1)
	  : mFunction(function), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		mFunction(mArg1);
	}

	FunctionPointer mFunction;
	Arg1 mArg1;
};

template <typename T, typename FunctionPointer, typename Arg1>
struct StoredFunctorPointerCall1 : public Functor<T>
{
	inline StoredFunctorPointerCall1(FunctionPointer *function, Arg1 const &arg1)
	  : mFunction(function), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		this->mResult =(*mFunction)(mArg1);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
};

template <typename T, typename FunctionPointer, typename Arg1>
struct VoidStoredFunctorPointerCall1 : public Functor<T>
{
	inline VoidStoredFunctorPointerCall1(FunctionPointer *function, Arg1 const &arg1)
	  : mFunction(function), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		(*mFunction)(mArg1);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
};

template <typename T, typename FunctionPointer, typename Arg1>
struct SelectStoredFunctorPointerCall1
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredFunctorPointerCall1    <T, FunctionPointer, Arg1>,
			 VoidStoredFunctorPointerCall1<T, FunctionPointer, Arg1> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1>
class StoredMemberFunctionCall1 : public Functor<T>
{
public:
	StoredMemberFunctionCall1(T (Class::*fn)(Param1) , Class const &object, Arg1 const &arg1)
	  : mFn(fn), mObject(object), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1);
	}

private:
	T (Class::*mFn)(Param1);
	Class mObject;
	Arg1 mArg1;
};

template <typename T, typename Class, typename Param1, typename Arg1>
class VoidStoredMemberFunctionCall1 : public Functor<T>
{
public:
	VoidStoredMemberFunctionCall1(T (Class::*fn)(Param1) , Class const &object, Arg1 const &arg1)
	  : mFn(fn), mObject(object), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		(mObject.*mFn)(mArg1);
	}

private:
	T (Class::*mFn)(Param1);
	Class mObject;
	Arg1 mArg1;
};

template <typename T, typename Class, typename Param1, typename Arg1>
struct SelectStoredMemberFunctionCall1
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionCall1    <T, Class, Param1, Arg1>,
			 VoidStoredMemberFunctionCall1<T, Class, Param1, Arg1> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1>
class StoredConstMemberFunctionCall1 : public Functor<T>
{
public:
	StoredConstMemberFunctionCall1(T (Class::*fn)(Param1) const, Class const &object, Arg1 const &arg1)
	  : mFn(fn), mObject(object), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1);
	}

private:
	T (Class::*mFn)(Param1) const;
	Class const mObject;
	Arg1 mArg1;
};

template <typename T, typename Class, typename Param1, typename Arg1>
class VoidStoredConstMemberFunctionCall1 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionCall1(T (Class::*fn)(Param1) const, Class const &object, Arg1 const &arg1)
	  : mFn(fn), mObject(object), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		(mObject.*mFn)(mArg1);
	}

private:
	T (Class::*mFn)(Param1) const;
	Class const mObject;
	Arg1 mArg1;
};

template <typename T, typename Class, typename Param1, typename Arg1>
struct SelectStoredConstMemberFunctionCall1
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionCall1    <T, Class, Param1, Arg1>,
			 VoidStoredConstMemberFunctionCall1<T, Class, Param1, Arg1> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1>
class StoredMemberFunctionPointerCall1 : public Functor<T>
{
public:
	StoredMemberFunctionPointerCall1(T (Class::*fn)(Param1) , Class *object, Arg1 const &arg1)
	  : mFn(fn), mObject(object), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1);
	}

private:
	T (Class::*mFn)(Param1);
	Class *mObject;
	Arg1 mArg1;
};

template <typename T, typename Class, typename Param1, typename Arg1>
class VoidStoredMemberFunctionPointerCall1 : public Functor<T>
{
public:
	VoidStoredMemberFunctionPointerCall1(T (Class::*fn)(Param1) , Class *object, Arg1 const &arg1)
	  : mFn(fn), mObject(object), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1);
	}

private:
	T (Class::*mFn)(Param1);
	Class *mObject;
	Arg1 mArg1;
};

template <typename T, typename Class, typename Param1, typename Arg1>
struct SelectStoredMemberFunctionPointerCall1
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionPointerCall1    <T, Class, Param1, Arg1>,
			 VoidStoredMemberFunctionPointerCall1<T, Class, Param1, Arg1> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1>
class StoredConstMemberFunctionPointerCall1 : public Functor<T>
{
public:
	StoredConstMemberFunctionPointerCall1(T (Class::*fn)(Param1) const, Class const *object, Arg1 const &arg1)
	  : mFn(fn), mObject(object), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1);
	}

private:
	T (Class::*mFn)(Param1) const;
	Class const *mObject;
	Arg1 mArg1;
};

template <typename T, typename Class, typename Param1, typename Arg1>
class VoidStoredConstMemberFunctionPointerCall1 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionPointerCall1(T (Class::*fn)(Param1) const, Class const *object, Arg1 const &arg1)
	  : mFn(fn), mObject(object), mArg1(arg1)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1);
	}

private:
	T (Class::*mFn)(Param1) const;
	Class const *mObject;
	Arg1 mArg1;
};

template <typename T, typename Class, typename Param1, typename Arg1>
struct SelectStoredConstMemberFunctionPointerCall1
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionPointerCall1    <T, Class, Param1, Arg1>,
			 VoidStoredConstMemberFunctionPointerCall1<T, Class, Param1, Arg1> >::type type;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2>
struct StoredFunctorCall2 : public Functor<T>
{
	inline StoredFunctorCall2(FunctionPointer function, Arg1 const &arg1, Arg2 const &arg2)
	  : mFunction(function), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		this->mResult = mFunction(mArg1, mArg2);
	}

	FunctionPointer mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename FunctionPointer, typename Arg1, typename Arg2>
struct StoredFunctorCall2<void, FunctionPointer, Arg1, Arg2> : public Functor<void>
{
	inline StoredFunctorCall2(FunctionPointer function, Arg1 const &arg1, Arg2 const &arg2)
	  : mFunction(function), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		mFunction(mArg1, mArg2);
	}

	FunctionPointer mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2>
struct StoredFunctorPointerCall2 : public Functor<T>
{
	inline StoredFunctorPointerCall2(FunctionPointer *function, Arg1 const &arg1, Arg2 const &arg2)
	  : mFunction(function), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		this->mResult =(*mFunction)(mArg1, mArg2);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2>
struct VoidStoredFunctorPointerCall2 : public Functor<T>
{
	inline VoidStoredFunctorPointerCall2(FunctionPointer *function, Arg1 const &arg1, Arg2 const &arg2)
	  : mFunction(function), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		(*mFunction)(mArg1, mArg2);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2>
struct SelectStoredFunctorPointerCall2
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredFunctorPointerCall2    <T, FunctionPointer, Arg1, Arg2>,
			 VoidStoredFunctorPointerCall2<T, FunctionPointer, Arg1, Arg2> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
class StoredMemberFunctionCall2 : public Functor<T>
{
public:
	StoredMemberFunctionCall2(T (Class::*fn)(Param1, Param2) , Class const &object, Arg1 const &arg1, Arg2 const &arg2)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1, mArg2);
	}

private:
	T (Class::*mFn)(Param1, Param2);
	Class mObject;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
class VoidStoredMemberFunctionCall2 : public Functor<T>
{
public:
	VoidStoredMemberFunctionCall2(T (Class::*fn)(Param1, Param2) , Class const &object, Arg1 const &arg1, Arg2 const &arg2)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		(mObject.*mFn)(mArg1, mArg2);
	}

private:
	T (Class::*mFn)(Param1, Param2);
	Class mObject;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
struct SelectStoredMemberFunctionCall2
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionCall2    <T, Class, Param1, Arg1, Param2, Arg2>,
			 VoidStoredMemberFunctionCall2<T, Class, Param1, Arg1, Param2, Arg2> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
class StoredConstMemberFunctionCall2 : public Functor<T>
{
public:
	StoredConstMemberFunctionCall2(T (Class::*fn)(Param1, Param2) const, Class const &object, Arg1 const &arg1, Arg2 const &arg2)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1, mArg2);
	}

private:
	T (Class::*mFn)(Param1, Param2) const;
	Class const mObject;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
class VoidStoredConstMemberFunctionCall2 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionCall2(T (Class::*fn)(Param1, Param2) const, Class const &object, Arg1 const &arg1, Arg2 const &arg2)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		(mObject.*mFn)(mArg1, mArg2);
	}

private:
	T (Class::*mFn)(Param1, Param2) const;
	Class const mObject;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
struct SelectStoredConstMemberFunctionCall2
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionCall2    <T, Class, Param1, Arg1, Param2, Arg2>,
			 VoidStoredConstMemberFunctionCall2<T, Class, Param1, Arg1, Param2, Arg2> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
class StoredMemberFunctionPointerCall2 : public Functor<T>
{
public:
	StoredMemberFunctionPointerCall2(T (Class::*fn)(Param1, Param2) , Class *object, Arg1 const &arg1, Arg2 const &arg2)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1, mArg2);
	}

private:
	T (Class::*mFn)(Param1, Param2);
	Class *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
class VoidStoredMemberFunctionPointerCall2 : public Functor<T>
{
public:
	VoidStoredMemberFunctionPointerCall2(T (Class::*fn)(Param1, Param2) , Class *object, Arg1 const &arg1, Arg2 const &arg2)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1, mArg2);
	}

private:
	T (Class::*mFn)(Param1, Param2);
	Class *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
struct SelectStoredMemberFunctionPointerCall2
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionPointerCall2    <T, Class, Param1, Arg1, Param2, Arg2>,
			 VoidStoredMemberFunctionPointerCall2<T, Class, Param1, Arg1, Param2, Arg2> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
class StoredConstMemberFunctionPointerCall2 : public Functor<T>
{
public:
	StoredConstMemberFunctionPointerCall2(T (Class::*fn)(Param1, Param2) const, Class const *object, Arg1 const &arg1, Arg2 const &arg2)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1, mArg2);
	}

private:
	T (Class::*mFn)(Param1, Param2)const;
	Class const *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
class VoidStoredConstMemberFunctionPointerCall2 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionPointerCall2(T (Class::*fn)(Param1, Param2) const, Class const *object, Arg1 const &arg1, Arg2 const &arg2)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1, mArg2);
	}

private:
	T (Class::*mFn)(Param1, Param2)const;
	Class const *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
struct SelectStoredConstMemberFunctionPointerCall2
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionPointerCall2    <T, Class, Param1, Arg1, Param2, Arg2>,
			 VoidStoredConstMemberFunctionPointerCall2<T, Class, Param1, Arg1, Param2, Arg2> >::type type;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3>
struct StoredFunctorCall3 : public Functor<T>
{
	inline StoredFunctorCall3(FunctionPointer function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		this->mResult = mFunction(mArg1, mArg2, mArg3);
	}

	FunctionPointer mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3>
struct StoredFunctorCall3<void, FunctionPointer, Arg1, Arg2, Arg3>: public Functor<void>
{
	inline StoredFunctorCall3(FunctionPointer function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		mFunction(mArg1, mArg2, mArg3);
	}

	FunctionPointer mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3>
struct StoredFunctorPointerCall3: public Functor<T>
{
	inline StoredFunctorPointerCall3(FunctionPointer *function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		this->mResult =(*mFunction)(mArg1, mArg2, mArg3);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3>
struct VoidStoredFunctorPointerCall3: public Functor<T>
{
	inline VoidStoredFunctorPointerCall3(FunctionPointer *function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		(*mFunction)(mArg1, mArg2, mArg3);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3>
struct SelectStoredFunctorPointerCall3
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredFunctorPointerCall3    <T, FunctionPointer, Arg1, Arg2, Arg3>,
			 VoidStoredFunctorPointerCall3<T, FunctionPointer, Arg1, Arg2, Arg3> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
class StoredMemberFunctionCall3 : public Functor<T>
{
public:
	StoredMemberFunctionCall3(T (Class::*fn)(Param1, Param2, Param3) , Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1, mArg2, mArg3);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3);
	Class mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
class VoidStoredMemberFunctionCall3 : public Functor<T>
{
public:
	VoidStoredMemberFunctionCall3(T (Class::*fn)(Param1, Param2, Param3) , Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		(mObject.*mFn)(mArg1, mArg2, mArg3);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3);
	Class mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
struct SelectStoredMemberFunctionCall3
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionCall3    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>,
			 VoidStoredMemberFunctionCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
class StoredConstMemberFunctionCall3 : public Functor<T>
{
public:
	StoredConstMemberFunctionCall3(T (Class::*fn)(Param1, Param2, Param3) const, Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1, mArg2, mArg3);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3) const;
	Class const mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
class VoidStoredConstMemberFunctionCall3 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionCall3(T (Class::*fn)(Param1, Param2, Param3) const, Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		(mObject.*mFn)(mArg1, mArg2, mArg3);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3) const;
	Class const mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
struct SelectStoredConstMemberFunctionCall3
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionCall3    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>,
			 VoidStoredConstMemberFunctionCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
class StoredMemberFunctionPointerCall3 : public Functor<T>
{
public:
	StoredMemberFunctionPointerCall3(T (Class::*fn)(Param1, Param2, Param3) , Class *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1, mArg2, mArg3);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3);
	Class *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
class VoidStoredMemberFunctionPointerCall3 : public Functor<T>
{
public:
	VoidStoredMemberFunctionPointerCall3(T (Class::*fn)(Param1, Param2, Param3) , Class *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1, mArg2, mArg3);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3);
	Class *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
struct SelectStoredMemberFunctionPointerCall3
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionPointerCall3    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>,
			 VoidStoredMemberFunctionPointerCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
class StoredConstMemberFunctionPointerCall3 : public Functor<T>
{
public:
	StoredConstMemberFunctionPointerCall3(T (Class::*fn)(Param1, Param2, Param3) const, Class const *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1, mArg2, mArg3);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3) const;
	Class const *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
class VoidStoredConstMemberFunctionPointerCall3 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionPointerCall3(T (Class::*fn)(Param1, Param2, Param3) const, Class const *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1, mArg2, mArg3);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3) const;
	Class const *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
struct SelectStoredConstMemberFunctionPointerCall3
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionPointerCall3    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>,
			 VoidStoredConstMemberFunctionPointerCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3> >::type type;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct StoredFunctorCall4 : public Functor<T>
{
	inline StoredFunctorCall4(FunctionPointer function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		this->mResult = mFunction(mArg1, mArg2, mArg3, mArg4);
	}

	FunctionPointer mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct StoredFunctorCall4<void, FunctionPointer, Arg1, Arg2, Arg3, Arg4> : public Functor<void>
{
	inline StoredFunctorCall4(FunctionPointer function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		mFunction(mArg1, mArg2, mArg3, mArg4);
	}

	FunctionPointer mFunction;
	Arg1 mArg1; Arg2 mArg2; Arg3 mArg3; Arg4 mArg4;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct StoredFunctorPointerCall4 : public Functor<T>
{
	inline StoredFunctorPointerCall4(FunctionPointer *function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		this->mResult =(*mFunction)(mArg1, mArg2, mArg3, mArg4);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct VoidStoredFunctorPointerCall4 : public Functor<T>
{
	inline VoidStoredFunctorPointerCall4(FunctionPointer *function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		(*mFunction)(mArg1, mArg2, mArg3, mArg4);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct SelectStoredFunctorPointerCall4
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredFunctorPointerCall4    <T, FunctionPointer, Arg1, Arg2, Arg3, Arg4>,
			 VoidStoredFunctorPointerCall4<T, FunctionPointer, Arg1, Arg2, Arg3, Arg4> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
class StoredMemberFunctionCall4 : public Functor<T>
{
public:
	StoredMemberFunctionCall4(T (Class::*fn)(Param1, Param2, Param3, Param4) , Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1, mArg2, mArg3, mArg4);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4);
	Class mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
class VoidStoredMemberFunctionCall4 : public Functor<T>
{
public:
	VoidStoredMemberFunctionCall4(T (Class::*fn)(Param1, Param2, Param3, Param4) , Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		(mObject.*mFn)(mArg1, mArg2, mArg3, mArg4);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4);
	Class mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
struct SelectStoredMemberFunctionCall4
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionCall4    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>,
			 VoidStoredMemberFunctionCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
class StoredConstMemberFunctionCall4 : public Functor<T>
{
public:
	StoredConstMemberFunctionCall4(T (Class::*fn)(Param1, Param2, Param3, Param4) const, Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1, mArg2, mArg3, mArg4);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4) const;
	Class const mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
class VoidStoredConstMemberFunctionCall4 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionCall4(T (Class::*fn)(Param1, Param2, Param3, Param4) const, Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		(mObject.*mFn)(mArg1, mArg2, mArg3, mArg4);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4) const;
	Class const mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
struct SelectStoredConstMemberFunctionCall4
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionCall4    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>,
			 VoidStoredConstMemberFunctionCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
class StoredMemberFunctionPointerCall4 : public Functor<T>
{
public:
	StoredMemberFunctionPointerCall4(T (Class::*fn)(Param1, Param2, Param3, Param4) , Class *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1, mArg2, mArg3, mArg4);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4);
	Class *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
class VoidStoredMemberFunctionPointerCall4 : public Functor<T>
{
public:
	VoidStoredMemberFunctionPointerCall4(T (Class::*fn)(Param1, Param2, Param3, Param4) , Class *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1, mArg2, mArg3, mArg4);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4);
	Class *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
struct SelectStoredMemberFunctionPointerCall4
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionPointerCall4    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>,
			 VoidStoredMemberFunctionPointerCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
class StoredConstMemberFunctionPointerCall4 : public Functor<T>
{
public:
	StoredConstMemberFunctionPointerCall4(T (Class::*fn)(Param1, Param2, Param3, Param4) const, Class const *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1, mArg2, mArg3, mArg4);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4)const;
	Class const *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
class VoidStoredConstMemberFunctionPointerCall4 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionPointerCall4(T (Class::*fn)(Param1, Param2, Param3, Param4) const, Class const *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1, mArg2, mArg3, mArg4);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4)const;
	Class const *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
struct SelectStoredConstMemberFunctionPointerCall4
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionPointerCall4    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>,
			 VoidStoredConstMemberFunctionPointerCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4> >::type type;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
struct StoredFunctorCall5 : public Functor<T>
{
	inline StoredFunctorCall5(FunctionPointer function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		this->mResult = mFunction(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

	FunctionPointer mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
struct StoredFunctorCall5<void, FunctionPointer, Arg1, Arg2, Arg3, Arg4, Arg5> : public Functor<void>
{
	inline StoredFunctorCall5(FunctionPointer function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		mFunction(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

	FunctionPointer mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
struct StoredFunctorPointerCall5 : public Functor<T>
{
	inline StoredFunctorPointerCall5(FunctionPointer *function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		this->mResult =(*mFunction)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
struct VoidStoredFunctorPointerCall5 : public Functor<T>
{
	inline VoidStoredFunctorPointerCall5(FunctionPointer *function, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFunction(function), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		(*mFunction)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

	FunctionPointer *mFunction;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename FunctionPointer, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
struct SelectStoredFunctorPointerCall5
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredFunctorPointerCall5    <T, FunctionPointer, Arg1, Arg2, Arg3, Arg4, Arg5>,
			 VoidStoredFunctorPointerCall5<T, FunctionPointer, Arg1, Arg2, Arg3, Arg4, Arg5> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
class StoredMemberFunctionCall5 : public Functor<T>
{
public:
	StoredMemberFunctionCall5(T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) , Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4, Param5);
	Class mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
class VoidStoredMemberFunctionCall5 : public Functor<T>
{
public:
	VoidStoredMemberFunctionCall5(T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) , Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		(mObject.*mFn)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4, Param5);
	Class mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
struct SelectStoredMemberFunctionCall5
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionCall5    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>,
			 VoidStoredMemberFunctionCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
class StoredConstMemberFunctionCall5 : public Functor<T>
{
public:
	StoredConstMemberFunctionCall5(T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) const, Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject.*mFn)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}
private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4, Param5) const;
	Class const mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
class VoidStoredConstMemberFunctionCall5 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionCall5(T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) const, Class const &object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5){ }

	void runFunctor()
	{
		(mObject.*mFn)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4, Param5) const;
	Class const mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
struct SelectStoredConstMemberFunctionCall5
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionCall5    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>,
			 VoidStoredConstMemberFunctionCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
class StoredMemberFunctionPointerCall5 : public Functor<T>
{
public:
	StoredMemberFunctionPointerCall5(T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) , Class *_object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFn(fn), mObject(_object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4, Param5);
	Class *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
class VoidStoredMemberFunctionPointerCall5 : public Functor<T>
{
public:
	VoidStoredMemberFunctionPointerCall5(T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) , Class *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4, Param5);
	Class *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
struct SelectStoredMemberFunctionPointerCall5
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredMemberFunctionPointerCall5    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>,
			 VoidStoredMemberFunctionPointerCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5> >::type type;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
class StoredConstMemberFunctionPointerCall5 : public Functor<T>
{
public:
	StoredConstMemberFunctionPointerCall5(T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) const, Class const *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		this->mResult = (mObject->*mFn)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4, Param5) const;
	Class const *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
class VoidStoredConstMemberFunctionPointerCall5 : public Functor<T>
{
public:
	VoidStoredConstMemberFunctionPointerCall5(T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) const, Class const *object, Arg1 const &arg1, Arg2 const &arg2, Arg3 const &arg3, Arg4 const &arg4, Arg5 const &arg5)
	  : mFn(fn), mObject(object), mArg1(arg1), mArg2(arg2), mArg3(arg3), mArg4(arg4), mArg5(arg5)
	{
	}

	void runFunctor()
	{
		(mObject->*mFn)(mArg1, mArg2, mArg3, mArg4, mArg5);
	}

private:
	T (Class::*mFn)(Param1, Param2, Param3, Param4, Param5) const;
	Class const *mObject;
	Arg1 mArg1;
	Arg2 mArg2;
	Arg3 mArg3;
	Arg4 mArg4;
	Arg5 mArg5;
};

template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
struct SelectStoredConstMemberFunctionPointerCall5
{
	typedef typename SelectSpecialization<T>::template
		Type<StoredConstMemberFunctionPointerCall5    <T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>,
			 VoidStoredConstMemberFunctionPointerCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5> >::type type;
};

}
}
