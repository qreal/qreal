#include "smartLine.h"

using namespace qReal;
using namespace generators;

SmartLine::SmartLine(QString text, qReal::Id elementId, IndentChangeType indentLevelChange)
	: mText(text), mElementId(elementId), mIndentChange(indentLevelChange)
{}

QString SmartLine::text() const {
	return mText;
}

qReal::Id SmartLine::elementId() const {
	return mElementId;
}

SmartLine::IndentChangeType SmartLine::indentLevelChange() const {
	return mIndentChange;
}

void SmartLine::setText(QString const &text) {
	mText = text;
}

void SmartLine::setElementId(qReal::Id const &id) {
	mElementId = id;
}

void SmartLine::setIndentChange(IndentChangeType t) {
	mIndentChange = t;
}
