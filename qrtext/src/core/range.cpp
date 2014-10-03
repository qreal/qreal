#include "qrtext/core/range.h"

using namespace qrtext::core;

Range::Range(Connection const &start, Connection const &end)
	: mStart(start), mEnd(end)
{
}

Range::Range()
{
}

Connection const &Range::start() const
{
	return mStart;
}

Connection const &Range::end() const
{
	return mEnd;
}

