#include "textLanguageParser/ast/connection.h"

using namespace textLanguageParser::ast;

Connection::Connection(int absolutePosition, int line, int column)
	: mAbsolutePosition(absolutePosition)
	, mLine(line)
	, mColumn(column)
{
}

Connection::Connection()
{
}

int Connection::absolutePosition() const
{
	return mAbsolutePosition;
}

int Connection::line() const
{
	return mLine;
}

int Connection::column() const
{
	return mColumn;
}
