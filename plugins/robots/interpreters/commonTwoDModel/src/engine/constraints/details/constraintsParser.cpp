#include "constraintsParser.h"

#include <QtXml/QDomDocument>

using namespace twoDModel::constraints::details;

ConstraintsParser::ConstraintsParser(Events &events, Variables &variables)
	: mEvents(events)
	, mVariables(variables)
{
}

QString ConstraintsParser::errorString() const
{
	return mError;
}

bool ConstraintsParser::parse(const QString &constraintsXml)
{
	mError = QString();

	QDomDocument document;
	QString errorMessage;
	int errorLine, errorColumn;
	if (!document.setContent(constraintsXml, &errorMessage, &errorLine, &errorColumn)) {
		mError = QString("%1:%2: %3").arg(QString::number(errorLine), QString::number(errorColumn), errorMessage);
		return false;
	}

	if (document.documentElement().tagName().toLower() != "constraints") {
		mError = QObject::tr("Root element must be \"constraints\" tag");
		return false;
	}
}

void ConstraintsParser::parseConstraints(const QDomElement &constraints)
{
	for (QDomElement constraint = constraints.firstChildElement()
			; !constraint.isNull()
			; constraint = constraint.nextSiblingElement())
	{
		const QString name = constraint.tagName().toLower();
		if (name == "event") {

		} else if (name == "constraint") {
			// Constraint is just an event with fail trigger.
			// Check-once constraint is an event with 0 timeout forcing to drop.

		} else if (name == "timelimit") {
			// Timelimit is just an event with timeout and fail trigger.
			QString timeLimitMessage = QObject::tr("Program worked for too long time");
		}
	}
}

