#include "smartLine.h"

using namespace robots::generator;

SmartLine_old::SmartLine_old(QString const &text, qReal::Id const &elementId, IndentChangeType indentLevelChange)
		: mText(text)
		, mElementId(elementId)
		, mIndentChange(indentLevelChange)
{
}

QString SmartLine_old::text() const
{
	return mText;
}

qReal::Id SmartLine_old::elementId() const
{
	return mElementId;
}

SmartLine_old::IndentChangeType SmartLine_old::indentLevelChange() const
{
	return mIndentChange;
}

void SmartLine_old::setText(QString const &text)
{
	mText = text;
}

void SmartLine_old::setElementId(qReal::Id const &id)
{
	mElementId = id;
}

void SmartLine_old::setIndentChange(IndentChangeType t)
{
	mIndentChange = t;
}
