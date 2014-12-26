#include "explosion.h"

using namespace qReal;

Explosion::Explosion(Id const &source, Id const &target
		, bool isReusable, bool requiresImmediateLinkage)
	: mSource(source), mTarget(target), mReusable(isReusable)
	, mRequiresImmediateLinkage(requiresImmediateLinkage)
{
}

Id Explosion::source() const
{
	return mSource;
}

Id Explosion::target() const
{
	return mTarget;
}

bool Explosion::isReusable() const
{
	return mReusable;
}

bool Explosion::requiresImmediateLinkage() const
{
	return mRequiresImmediateLinkage;
}
