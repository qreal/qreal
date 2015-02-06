#pragma once

#include "defines.h"
#include "triggersFactory.h"
#include "conditionsFactory.h"
#include "valuesFactory.h"

class QDomElement;

namespace twoDModel {
namespace constraints {
namespace details {

class ConstraintsParser
{
public:
	ConstraintsParser(Events &events
		, Variables &variables
		, const Objects &objects
		, const utils::TimelineInterface &timeline);

	bool parse(const QString &constrtaintsXml);
	QStringList errors() const;

private:
	void parseConstraints(const QDomElement &constraints);
	Event *parseConstraint(const QDomElement &constraint);

	Event *parseEventTag(const QDomElement &element);
	Event *parseConstraintTag(const QDomElement &element);
	Event *parseTimeLimitTag(const QDomElement &element);

	Condition parseConditionsAlternative(const QDomElement &element);
	Condition parseConditionsTag(const QDomElement &element);
	Condition parseConditionTag(const QDomElement &element);
	Condition parseConditionContents(const QDomElement &element);

	Condition parseNegationTag(const QDomElement &element);
	Condition parseComparisonTag(const QDomElement &element);
	Condition parseInsideTag(const QDomElement &element);
	Condition parseEventSettedDroppedTag(const QDomElement &element);
	Condition parseTimerTag(const QDomElement &element);

	Trigger parseTriggersTag(const QDomElement &element);
	Trigger parseTriggerTag(const QDomElement &element);
	Trigger parseTriggerContents(const QDomElement &element);

	Trigger parseFailTag(const QDomElement &element);
	Trigger parseSuccessTag(const QDomElement &element);
	Trigger parseSetVariableTag(const QDomElement &element);
	Trigger parseAddToVariableTag(const QDomElement &element);
	Trigger parseEventSetDropTag(const QDomElement &element);

	QString id(const QDomElement &element) const;
	int intAttribute(const QDomElement &element, QString const &attributeName, int defaultValue = -1);
	QVariant bestVariant(const QString &value) const;

	bool addToEvents(Event * const event);
	bool assertChildrenExactly(const QDomElement &element, int count);
	bool assertChildrenMoreThan(const QDomElement &element, int count);
	bool assertHasAttribute(const QDomElement &element, const QString &attribute);
	bool assertTagName(const QDomElement &element, const QString &nameInLowerCase);
	bool assertAttributeNonEmpty(const QDomElement &element, const QString &attribute);

	bool error(const QString &message);

	QStringList mErrors;

	Events &mEvents;
	Variables &mVariables;
	const Objects &mObjects;
	const utils::TimelineInterface &mTimeline;

	const TriggersFactory mTriggers;
	const ConditionsFactory mConditions;
	const ValuesFactory mValues;
};

}
}
}
