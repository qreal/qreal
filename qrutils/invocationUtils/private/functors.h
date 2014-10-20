#pragma once

#include <QtCore/qglobal.h>
#include <QtConcurrent/qtconcurrentcompilertest.h>
#include "../progress.h"

namespace invocation
{
namespace details
{

template<typename T1, typename T2>
struct TypesComparator
{
	enum
	{
		EQUAL = false
	};
};

template<typename T>
struct TypesComparator<T, T>
{
	enum
	{
		EQUAL = true
	};
};

template<int ...>
struct Sequence {};

template<int N, int ...Seq>
struct GenerateSequence : GenerateSequence<N - 1, N - 1, Seq...> {};

template<int ...Seq>
struct GenerateSequence<0, Seq...>
{
	typedef Sequence<Seq...> type;
};

template<typename Result>
class FunctorInterface
{
public:
	virtual void startFunc() = 0;
	virtual Result const result() = 0;
	virtual void setProgressIfNecessary(Progress *ptr) = 0;

	virtual bool isReturnSmth()
	{
		return true;
	}

	virtual bool withProgress()
	{
		return false;
	}
};

template <typename Result, typename Target, typename Func, typename ...Args>
class Functor : public FunctorInterface<Result>
{
public:
	Functor(Target *obj, Func f, Args ...data)
		:FunctorInterface<Result>(), mObj(obj), mf(f), mStore(std::tuple<Args...>(data...)){}

	Result const result()
	{
		return res;
	}

	void startFunc()
	{
		callFunc(typename GenerateSequence<sizeof...(Args)>::type());
	}

	void setProgressIfNecessary(Progress *ptr)
	{
		Q_UNUSED(ptr)
	}

private:
	template<int ...Seq>
	void callFunc(Sequence<Seq...>){
		res = (mObj->*mf)(std::get<Seq>(mStore)...);
	}

	Result res;
	Target *mObj;
	Func mf;
	std::tuple<Args...> mStore;
};

template <typename Result, typename Target, typename Func, typename ...Args>
class VoidFunctor : public FunctorInterface<Result>
{
public:
	VoidFunctor(Target *obj, Func f, Args ...data)
		:FunctorInterface<Result>(), mObj(obj), mf(f), mStore(std::tuple<Args...>(data...)){}
	Result const result(){}

	bool isReturnSmth()
	{
		return false;
	}

	void startFunc()
	{
		callFunc(typename GenerateSequence<sizeof...(Args)>::type());
	}

	void setProgressIfNecessary(Progress *ptr)
	{
		Q_UNUSED(ptr)
	}

private:
	template<int ...Seq>
	void callFunc(Sequence<Seq...>)
	{
		(mObj->*mf)(std::get<Seq>(mStore)...);
	}

	Target *mObj;
	Func mf;
	std::tuple<Args...> mStore;
};

template <typename Result, typename Target, typename Func, typename ...Args>
class FunctorWithProgress : public FunctorInterface<Result>
{
public:
	FunctorWithProgress(Target *obj, Func f, Args ...data)
		:FunctorInterface<Result>(), mObj(obj), mf(f), mStore(std::tuple<Args...>(data...)){}

	Result const result()
	{
		return res;
	}

	void startFunc()
	{
		callFunc(typename GenerateSequence<sizeof...(Args)>::type());
	}

	bool withProgress()
	{
		return true;
	}

	void setProgressIfNecessary(Progress *ptr)
	{
		std::get<0>(mStore) = ptr;
	}

private:
	template<int ...Seq>
	void callFunc(Sequence<Seq...>){
		res = (mObj->*mf)(std::get<Seq>(mStore)...);
	}

	Result res;
	Target *mObj;
	Func mf;
	std::tuple<Args...> mStore;
};

template <typename Result, typename Target, typename Func, typename ...Args>
class VoidFunctorWithProgress : public FunctorInterface<Result>
{
public:
	VoidFunctorWithProgress(Target *obj, Func f, Args ...data)
		:FunctorInterface<Result>(), mObj(obj), mf(f), mStore(std::tuple<Args...>(data...)){}
	Result const result(){}

	bool isReturnSmth()
	{
		return false;
	}

	void startFunc()
	{
		callFunc(typename GenerateSequence<sizeof...(Args)>::type());
	}


	bool withProgress()
	{
		return true;
	}

	void setProgressIfNecessary(Progress *ptr)
	{
		std::get<0>(mStore) = ptr;
	}

private:
	template<int ...Seq>
	void callFunc(Sequence<Seq...>)
	{
		(mObj->*mf)(std::get<Seq>(mStore)...);
	}

	Target *mObj;
	Func mf;
	std::tuple<Args...> mStore;
};

}
}
