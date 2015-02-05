#include "constraintsChecker.h"

#include "details/constraintsParser.h"

using namespace twoDModel::constraints;

ConstraintsChecker::~ConstraintsChecker()
{
	qDeleteAll(mEvents);
}

#include<QDebug>
bool ConstraintsChecker::parseConstraints(const QString &constraintsXml)
{
	qDeleteAll(mEvents);
	mEvents.clear();
	mVariables.clear();
	mObjects.clear();

	details::ConstraintsParser parser(mEvents, mVariables);
	if (!parser.parse(constraintsXml)) {
		qDebug() << parser.errorString();
		return false;
	}

	return true;
}
