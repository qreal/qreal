/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include "defines.h"
#include "triggersFactory.h"
#include "conditionsFactory.h"
#include "valuesFactory.h"

class QDomElement;

namespace twoDModel {
namespace constraints {
namespace details {

/// @brief Parser for 2D model constraints language.
///
/// Program on 2D model constraints language is an XML-based description of allowed robot`s behaviour in the concrete
/// world model. It is simply a number of events that trigger some actions, set up and drop each other.
/// Each event is an entity consisting of two parts: condition and trigger. Condition is a boolean description of the
/// moment when the event must be fired. Triggers are the actions that must be invoked then. By default the event will
/// be dropped (i.e. its listening will be stopped by system) when it is fired. "dropsOnFire" attribute may be used to
/// disable it. The id may be assigned to event for setting it up or dropping it manually in other event triggers.
/// Each program must have (and only one) timelimit constraint.
/// Constraints can also be used in programs. Tag "constraint" is simply a event with fail trigger and condition
/// that is a negation of specified one. Constraint tags must have "failMessage" attribute therefore and its child
/// tag is always a condition.
/// Full list of possible conditions and triggers will not be specified here because it is supposed to be oftenly
/// extended. You can find it out looking into private methods signatures.
class ConstraintsParser
{
public:
	ConstraintsParser(Events &events
		, Variables &variables
		, const Objects &objects
		, const utils::TimelineInterface &timeline
		, StatusReporter &status);

	/// Parses the given program, initializes events map given in constructor.
	/// Returns true if parsing process completed successfully. Otherwise returns false. The errors list in that case
	/// can be obtained via errors() method.
	bool parse(const QString &constraintsXml);

	/// Parses the given program, initializes events map given in constructor.
	/// Returns true if parsing process completed successfully. Otherwise returns false. The errors list in that case
	/// can be obtained via errors() method.
	bool parse(const QDomElement &constraintsXml);

	/// Returns a list of parser errors occured during the last parsing process.
	QStringList errors() const;

private:
	bool parseConstraints(const QDomElement &constraints);
	Event *parseConstraint(const QDomElement &constraint);

	Event *parseEventTag(const QDomElement &element);
	Event *parseConstraintTag(const QDomElement &element);
	Event *parseTimeLimitTag(const QDomElement &element);
	Event *parseInitializationTag(const QDomElement &element);

	Condition parseConditionsAlternative(const QDomElement &element, Event &event);
	Condition parseConditionsTag(const QDomElement &element, Event &event);
	Condition parseConditionTag(const QDomElement &element, Event &event);
	Condition parseConditionContents(const QDomElement &element, Event &event);

	Condition parseNegationTag(const QDomElement &element, Event &event);
	Condition parseComparisonTag(const QDomElement &element);
	Condition parseInsideTag(const QDomElement &element);
	Condition parseEventSettedDroppedTag(const QDomElement &element);
	Condition parseTimerTag(const QDomElement &element, Event &event);
	Condition parseUsingTag(const QDomElement &element, Event &event);

	Trigger parseTriggersAlternative(const QDomElement &element);
	Trigger parseTriggersTag(const QDomElement &element);
	Trigger parseTriggerTag(const QDomElement &element);
	Trigger parseTriggerContents(const QDomElement &element);

	Trigger parseFailTag(const QDomElement &element);
	Trigger parseSuccessTag(const QDomElement &element);
	Trigger parseSetterTag(const QDomElement &element);
	Trigger parseEventSetDropTag(const QDomElement &element);
	Trigger parseSetObjectStateTag(const QDomElement &element);

	Value parseValue(const QDomElement &element);
	Value parseBoolTag(const QDomElement &element);
	Value parseIntTag(const QDomElement &element);
	Value parseDoubleTag(const QDomElement &element);
	Value parseStringTag(const QDomElement &element);
	Value parseVariableValueTag(const QDomElement &element);
	Value parseTypeOfTag(const QDomElement &element);
	Value parseObjectStateTag(const QDomElement &element);
	Value parseUnaryValueTag(const QDomElement &element);
	Value parseBinaryValueTag(const QDomElement &element);

	QString id(const QDomElement &element) const;
	int intAttribute(const QDomElement &element, const QString &attributeName, int defaultValue = -1);
	qreal doubleAttribute(const QDomElement &element, const QString &attributeName, qreal defaultValue = 0.0);
	bool boolAttribute(const QDomElement &element, const QString &attributeName, bool defaultValue = false);

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
