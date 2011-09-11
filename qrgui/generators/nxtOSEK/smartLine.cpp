#include "smartLine.h"

using namespace qReal;
using namespace generators;

SmartLine::SmartLine(QString text, qReal::Id elementId, TabLevelChangeType tabLevelChange)
	: mText(text), mElementId(elementId), mTabLevelChange(tabLevelChange)
{}

QString SmartLine::text() const {
	return mText;
}

qReal::Id SmartLine::elementId() const {
	return mElementId;
}

SmartLine::TabLevelChangeType SmartLine::tabLevelChange() const {
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
