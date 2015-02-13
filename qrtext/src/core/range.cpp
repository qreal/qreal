#include "qrtext/core/range.h"

using namespace qrtext::core;

Range::Range(const Connection &start, const Connection &end)
	: mStart(start), mEnd(end)
{
}

Range::Range()
{
}

const Connection &Range::start() const
{
	return mStart;
}

const Connection &Range::end() const
{
	return mEnd;
}

