#include "constraintsParser.h"

#include <QtCore/QUuid>
#include <QtXml/QDomDocument>

#include "event.h"

using namespace twoDModel::constraints::details;

ConstraintsParser::ConstraintsParser(Events &events
		, Variables &variables
		, const Objects &objects
		, const utils::TimelineInterface &timeline)
	: mEvents(events)
	, mVariables(variables)
	, mObjects(objects)
	, mTimeline(timeline)
	, mTriggersFactory(mEvents, mVariables)
	, mConditionsFactory(mEvents, mVariables, mObjects)
	, mValuesFactory(mVariables, mObjects)
{
}

QStringList ConstraintsParser::errors() const
{
	return mErrors;
}

bool ConstraintsParser::parse(const QString &constraintsXml)
{
	mErrors.clear();

	if (constraintsXml.isEmpty()) {
		return true;
	}

	QDomDocument document;
	QString errorMessage;
	int errorLine, errorColumn;
	if (!document.setContent(constraintsXml, &errorMessage, &errorLine, &errorColumn)) {
		mErrors << QString("%1:%2: %3").arg(QString::number(errorLine), QString::number(errorColumn), errorMessage);
		return false;
	}

	if (document.documentElement().tagName().toLower() != "constraints") {
		mErrors << QObject::tr("Root element must be \"constraints\" tag");
		return false;
	}

	return true;
}

void ConstraintsParser::parseConstraints(const QDomElement &constraints)
{
	int timelimitTagsCount = 0;
	for (QDomElement constraint = constraints.firstChildElement()
			; !constraint.isNull()
			; constraint = constraint.nextSiblingElement())
	{
		if (!addToEvents(parseConstraint(constraint))) {
			return;
		}

		if (constraint.tagName().toLower() == "timelimit") {
			++timelimitTagsCount;
		}
	}

	if (timelimitTagsCount == 0) {
		error(QObject::tr("There must be a \"timelimit\" constraint."));
	}

	if (timelimitTagsCount > 1) {
		error(QObject::tr("There must be only one \"timelimit\" tag."));
	}
}

Event *ConstraintsParser::parseConstraint(const QDomElement &constraint)
{
	const QString name = constraint.tagName().toLower();

	if (name == "event") {

	}

	if (name == "constraint") {
	}

	if (name == "timelimit") {
		return parseTimeLimitTag(constraint);
	}

	/// @todo: Display unknown tag errors
	return nullptr;
}

Event *ConstraintsParser::parseEventTag(const QDomElement &element)
{
	if (!assertChildrenCount(element, 2)) {
		return nullptr;
	}

	const QDomElement child1 = element.firstChildElement();
	const QDomElement child2 = child1.nextSiblingElement();
	const QString child1Name = child1.tagName().toLower();
	const QString child2Name = child2.tagName().toLower();

	const bool firstIsCondition = child1Name == "condition" || child1Name == "conditions";
	const QDomElement conditionTag = firstIsCondition ? child1 : child2;
	const QDomElement triggerTag = firstIsCondition ? child2 : child1;
	const QString conditionName = firstIsCondition ? child1Name : child2Name;
	const QString triggerName = firstIsCondition ? child2Name : child1Name;

	if (conditionName != "condition" && conditionName != "conditions") {
		error(QObject::tr("Event tag must have \"condition\" or \"conditions\" child tag. \"%1\" found instead.")
				.arg(conditionTag.tagName()));
		return nullptr;
	}

	if (triggerName != "trigger" && triggerName != "triggers") {
		error(QObject::tr("Event tag must have \"trigger\" or \"triggers\" child tag. \"%1\" found instead.")
				.arg(triggerTag.tagName()));
		return nullptr;
	}

	const Condition condition = conditionName == "condition"
			? parseConditionTag(conditionTag)
			: parseConditionTag(conditionTag);
	const Trigger trigger = triggerName == "trigger"
			? parseTriggerTag(triggerTag)
			: parseTriggersTag(triggerTag);

	const QString setUpInitiallyAttribute = element.attribute("settedUpInitially", "false").toLower();
	const QString dropsOnFireAttribute = element.attribute("dropsOnFire", "true").toLower();
	const bool setUpInitially = setUpInitiallyAttribute == "true";
	const bool dropsOnFire = dropsOnFireAttribute == "true";

	Event * const result = new Event(id(element), condition, trigger, dropsOnFire);
	if (setUpInitially) {
		result->setUp();
	}

	return result;
}

Event *ConstraintsParser::parseConstraintTag(const QDomElement &element)
{
	// Constraint is just an event with fail trigger.
	// Check-once constraint is an event with 0 timeout forcing to drop.

	if (!assertChildrenCount(element, 1)) {
		return nullptr;
	}

	if (!element.hasAttribute("failMessage")) {
		error(QObject::tr("\"Constraint\" tag must have \"failMessage\" attribute."));
		return nullptr;
	}

	const QDomElement child = element.firstChildElement();
	const QString childName = child.tagName().toLower();

	Condition condition = childName == "conditions"
			? parseConditionsTag(child)
			: childName == "condition"
					? parseConditionTag(element)
					: parseConditionContents(element);

	const QString failMessage = element.attribute("failMessage");
	const Trigger trigger = mTriggersFactory.fail(failMessage);

	const QString checkOneAttribute = element.attribute("checkOnce", "false").toLower();
	const bool checkOnce = checkOneAttribute == "true";

	Event * const result = new Event(id(element), mConditionsFactory.constant(true), trigger);

	if (checkOnce) {
		const Value timestamp = mValuesFactory.timestamp(mTimeline);
		const Condition timeout = mConditionsFactory.timerCondition(0, true, timestamp, *result);
		condition = mConditionsFactory.combined({ timeout, condition }, Glue::And);
	}

	result->setCondition(condition);
	result->setUp();
	return result;
}

Event *ConstraintsParser::parseTimeLimitTag(const QDomElement &element)
{
	// Timelimit is just an event with timeout and fail trigger.
	const int value = intAttribute(element, "value");
	if (value < 0) {
		return nullptr;
	}

	const QString timeLimitMessage = QObject::tr("Program worked for too long time");
	const Value timestamp = mValuesFactory.timestamp(mTimeline);
	Event * const event = new Event(id(element)
			, mConditionsFactory.constant(true)
			, mTriggersFactory.fail(timeLimitMessage)
			, true);

	const Condition condition = mConditionsFactory.timerCondition(value, true, timestamp, *event);
	event->setCondition(condition);
	event->setUp();

	return event;
}

Condition ConstraintsParser::parseConditionsTag(const QDomElement &element) const
{

}

Condition ConstraintsParser::parseConditionTag(const QDomElement &element) const
{

}

Condition ConstraintsParser::parseConditionContents(const QDomElement &element) const
{

}

Trigger ConstraintsParser::parseTriggersTag(const QDomElement &element) const
{

}

Trigger ConstraintsParser::parseTriggerTag(const QDomElement &element) const
{

}

QString ConstraintsParser::id(const QDomElement &element) const
{
	const QString attribute = element.attribute("id");
	const QString id = attribute.isEmpty() ? QUuid::createUuid().toString() : attribute;
	return id;
}

int ConstraintsParser::intAttribute(const QDomElement &element, const QString &attributeName, int defaultValue)
{
	QString const attributeValue = element.attribute(attributeName);
	bool ok = false;
	const int result = attributeValue.toInt(&ok);
	if (!ok) {
		error(QObject::tr("Invalid integer value \"%1\"").arg(attributeValue));
		/// @todo: Show error;
		return defaultValue;
	}

	return result;
}

bool ConstraintsParser::addToEvents(Event * const event)
{
	if (!event) {
		return false;
	}

	const QString id = event->id();
	if (mEvents.contains(id)) {
		return error(QObject::tr("Duplicate id: \"%1\"").arg(id));
	}

	mEvents[event->id()] = event;
	return true;
}

bool ConstraintsParser::assertChildrenCount(const QDomElement &element, int count)
{
	if (element.childNodes().count() != count) {
		return error(QObject::tr("%1 tag must have exactly %2 child tag(s)")
				.arg(element.tagName(), QString::number(count)));
	}

	return true;
}

bool ConstraintsParser::error(const QString &message)
{
	mErrors << message;
	return false;
}
