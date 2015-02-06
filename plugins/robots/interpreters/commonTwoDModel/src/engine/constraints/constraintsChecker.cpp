#include "constraintsChecker.h"

#include "details/constraintsParser.h"
#include "details/event.h"

using namespace twoDModel::constraints;

ConstraintsChecker::ConstraintsChecker()
{
}

ConstraintsChecker::~ConstraintsChecker()
{
	qDeleteAll(mEvents);
}

#include<QDebug>
#include<utils/realTimeline.h>
bool ConstraintsChecker::parseConstraints(const QString &constraintsXml)
{
	qDeleteAll(mEvents);
	mEvents.clear();
	mVariables.clear();
	mObjects.clear();

	utils::RealTimeline *timeline = new utils::RealTimeline;
	details::ConstraintsParser parser(mEvents, mVariables, mObjects, *timeline);
	if (!parser.parse(constraintsXml)) {
		qDebug() << parser.errors();
		return false;
	}

	return true;
}
