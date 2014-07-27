#include "textLanguageParser/ast/range.h"

using namespace textLanguageParser::ast;

Range::Range(int startAbsolutePosition, int startLine, int startColumn
		, int endAbsolutePosition, int endLine, int endColumn)
	: mStart(startAbsolutePosition, startLine, startColumn)
	, mEnd(endAbsolutePosition, endLine, endColumn)
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

