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
	if (!assertChildrenExactly(element, 2)) {
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

	if (!assertChildrenExactly(element, 1)) {
		return nullptr;
	}

	if (!assertHasAttribute(element, "failMessage")) {
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

Condition ConstraintsParser::parseConditionsTag(const QDomElement &element)
{
	if (!assertChildrenMoreThan(element, 0) || !assertHasAttribute(element, "glue")) {
		return mConditionsFactory.constant(true);
	}

	const QString glueAttribute = element.attribute("glue").toLower();
	Glue glue;
	if (glueAttribute == "and") {
		glue = Glue::And;
	} else if (glueAttribute == "or") {
		glue = Glue::Or;
	} else {
		error(QObject::tr("\"Glue\" attribute must have value \"and\" or \"or\"."));
		return mConditionsFactory.constant(true);
	}

	QList<Condition> conditions;
	for (QDomElement condition = element.firstChildElement()
			; !condition.isNull()
			; condition = condition.nextSiblingElement())
	{
		if (!assertTagName(condition, "condition")) {
			return mConditionsFactory.constant(true);
		}

		conditions << parseConditionsTag(condition);
	}

	return mConditionsFactory.combined(conditions, glue);
}

Condition ConstraintsParser::parseConditionTag(const QDomElement &element)
{
	if (!assertChildrenExactly(element, 1)) {
		return mConditionsFactory.constant(true);
	}

	return parseConditionContents(element);
}

Condition ConstraintsParser::parseConditionContents(const QDomElement &element)
{
	const QString tag = element.tagName();

	if (tag == "not") {
		return parseNegationTag(element);
	}

	if (tag == "equals" || tag == "notEquals"
			|| tag == "greater" || tag == "less"
			|| tag == "notGreater" || tag == "notLess")
	{
		return parseComparisonTag(element);
	}

	if (tag == "inside") {
		return parseInsideTag(element);
	}

	if (tag == "settedUp" || tag == "dropped") {
		return parseEventSettedDroppedTag(element);
	}

	if (tag == "timer") {
		return parseTimerTag(element);
	}

	error(QObject::tr("Unknown tag \"%1\".").arg(element.tagName()));
	return mConditionsFactory.constant(true);
}

Condition ConstraintsParser::parseNegationTag(const QDomElement &element)
{

}

Condition ConstraintsParser::parseComparisonTag(const QDomElement &element)
{

}

Condition ConstraintsParser::parseInsideTag(const QDomElement &element)
{

}

Condition ConstraintsParser::parseEventSettedDroppedTag(const QDomElement &element)
{

}

Condition ConstraintsParser::parseTimerTag(const QDomElement &element)
{

}

Trigger ConstraintsParser::parseTriggersTag(const QDomElement &element)
{
	if (!assertChildrenMoreThan(element, 0)) {
		return mTriggersFactory.doNothing();
	}

	QList<Trigger> triggers;
	for (QDomElement trigger = element.firstChildElement()
			; !trigger.isNull()
			; trigger = trigger.nextSiblingElement())
	{
		if (!assertTagName(trigger, "trigger")) {
			return mTriggersFactory.doNothing();
		}

		triggers << parseTriggerTag(trigger);
	}

	return mTriggersFactory.combined(triggers);
}

Trigger ConstraintsParser::parseTriggerTag(const QDomElement &element)
{
	if (!assertChildrenExactly(element, 1)) {
		return mTriggersFactory.doNothing();
	}

	return parseTriggerContents(element);
}

Trigger ConstraintsParser::parseTriggerContents(const QDomElement &element)
{
	const QString tag = element.tagName();

	if (tag == "fail") {
		return parseFailTag(element);
	}

	if (tag == "success") {
		return parseSuccessTag(element);
	}

	if (tag == "setVariable") {
		return parseSetVariableTag(element);
	}

	if (tag == "addToVariable") {
		return parseAddToVariableTag(element);
	}

	if (tag == "setUp" || tag == "drop") {
		return parseEventSetDropTag(element);
	}

	error(QObject::tr("Unknown tag \"%1\".").arg(element.tagName()));
	return mTriggersFactory.doNothing();
}

Trigger ConstraintsParser::parseFailTag(const QDomElement &element)
{

}

Trigger ConstraintsParser::parseSuccessTag(const QDomElement &element)
{

}

Trigger ConstraintsParser::parseSetVariableTag(const QDomElement &element)
{

}

Trigger ConstraintsParser::parseAddToVariableTag(const QDomElement &element)
{

}

Trigger ConstraintsParser::parseEventSetDropTag(const QDomElement &element)
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
	if (!assertHasAttribute(element, "value")) {
		return 0;
	}

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

bool ConstraintsParser::assertChildrenExactly(const QDomElement &element, int count)
{
	if (element.childNodes().count() != count) {
		return error(QObject::tr("%1 tag must have exactly %2 child tag(s)")
				.arg(element.tagName(), QString::number(count)));
	}

	return true;
}

bool ConstraintsParser::assertChildrenMoreThan(const QDomElement &element, int count)
{
	if (element.childNodes().count() <= count) {
		return error(QObject::tr("%1 tag must have at least %2 child tag(s)")
				.arg(element.tagName(), QString::number(count + 1)));
	}

	return true;
}

bool ConstraintsParser::assertHasAttribute(const QDomElement &element, const QString &attribute)
{
	if (!element.hasAttribute(attribute)) {
		error(QObject::tr("\"%1\" tag must have \"%2\" attribute.").arg(element.tagName(), attribute));
		return false;
	}

	return true;
}

bool ConstraintsParser::assertTagName(const QDomElement &element, const QString &nameInLowerCase)
{
	if (element.tagName().toLower() != nameInLowerCase) {
		error(QObject::tr("Expected \"%1\" tag, got \"%2\".").arg(nameInLowerCase, element.tagName()));
		return false;
	}

	return true;
}

bool ConstraintsParser::error(const QString &message)
{
	mErrors << message;
	return false;
}
