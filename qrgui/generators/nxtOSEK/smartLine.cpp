#include "smartLine.h"

using namespace qReal;
using namespace generators;

SmartLine::SmartLine(QString text, qReal::Id elementId, TabLevelChangeType tabLevelChange)
	: mText(text), mElementId(elementId), tabLevelChange(mTabLevelChange)
{}

QString SmartLine::text() {
	return mText;
}

qReal::Id SmartLine::elementId() {
	return mElementId;
}

TabLevelChangeType SmartLine::tabLevelChange() {
	return mTabLevelChange;
}

void SmartLine::setText(QString const &text) {
	mText = text;
}

void SmartLine::setElementId(qReal::Id const &id) {
	mElementId = id;
}

void SmartLine::setTabLevelChange(TabLevelChangeType t) {
	mTabLevelChange = t;
}
