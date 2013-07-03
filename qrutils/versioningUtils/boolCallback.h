#pragma once

#include "../utilsDeclSpec.h"

namespace invocation
{

/// Base for construction callbacks with bool param from class members
struct BoolCallback
{
	virtual ~BoolCallback() {}
	virtual void operator()(bool const param) = 0;
};

/// Class for construction callbacks with bool param
/// or bool param and string one or with bool param and string and bool one
// TODO: Get rid of this nightmare and use lambdas or smth
template<class TOwner>
class QRUTILS_EXPORT BoolClassMemberCallback : public BoolCallback
{
	typedef void (TOwner::*BoolMember)(bool const param);
	typedef void (TOwner::*BoolMemberWithStringTag)(bool const param, QString const &tag);
	typedef void (TOwner::*BoolMemberWithStringAndBoolTag)(bool const param, QString const &stringTag, bool const boolTag);

public:
	BoolClassMemberCallback(TOwner *ownerClass, BoolMember member)
		: mOwner(ownerClass), mIsTaggedWithString(false)
		, mIsTaggedWithBoolAndString(false), mMember(member)
	{
	}

	/// @param tag Will be called as second parameter. Just a way to store some info in callback
	BoolClassMemberCallback(TOwner *ownerClass, BoolMemberWithStringTag member, QString const &tag)
		: mOwner(ownerClass), mIsTaggedWithString(true)
		, mIsTaggedWithBoolAndString(false)
		, mStringTaggedMember(member), mStringTag(tag)
	{
	}

	/// @param tag Will be called as second parameter. Just a way to store some info in callback
	BoolClassMemberCallback(TOwner *ownerClass, BoolMemberWithStringAndBoolTag member
			, QString const &stringTag, bool const &boolTag)
		: mOwner(ownerClass), mIsTaggedWithString(false)
		, mIsTaggedWithBoolAndString(true)
		, mBoolStringTaggedMember(member), mStringTag(stringTag)
		, mBoolTag(boolTag)
	{
	}

	virtual void operator ()(bool const param)
	{
		if (mIsTaggedWithBoolAndString) {
			(mOwner->*mBoolStringTaggedMember)(param, mStringTag, mBoolTag);
		} else if (mIsTaggedWithString) {
			(mOwner->*mStringTaggedMember)(param, mStringTag);
		} else {
			(mOwner->*mMember)(param);
		}
	}

private:
	TOwner *mOwner;
	bool mIsTaggedWithString;
	bool mIsTaggedWithBoolAndString;
	BoolMember mMember;
	BoolMemberWithStringTag mStringTaggedMember;
	BoolMemberWithStringAndBoolTag mBoolStringTaggedMember;
	QString mStringTag;
	bool mBoolTag;
};

}
