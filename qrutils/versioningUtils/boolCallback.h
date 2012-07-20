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
/// or bool param and string one,
template<class TOwner>
class QRUTILS_EXPORT BoolClassMemberCallback : public BoolCallback
{
	typedef void (TOwner::*BoolMember)(bool const param);
	typedef void (TOwner::*BoolMemberWithTag)(bool const param, QString const &tag);

public:
	BoolClassMemberCallback(TOwner *ownerClass, BoolMember member)
		: mOwner(ownerClass), mIsTagged(false), mMember(member)
	{
	}

	/// @param tag Will be called as second parameter. Just a way to store some info in callback
	BoolClassMemberCallback(TOwner *ownerClass, BoolMemberWithTag member, QString const &tag)
		: mOwner(ownerClass), mIsTagged(true), mTaggedMember(member), mTag(tag)
	{
	}

	virtual void operator ()(bool const param)
	{
		if (mIsTagged) {
			(mOwner->*mTaggedMember)(param, mTag);
		} else {
			(mOwner->*mMember)(param);
		}
	}

private:
	TOwner *mOwner;
	bool mIsTagged;
	BoolMember mMember;
	BoolMemberWithTag mTaggedMember;
	QString mTag;
};

}
