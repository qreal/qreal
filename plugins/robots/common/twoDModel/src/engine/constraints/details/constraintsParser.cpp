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

#include "constraintsParser.h"

#include <QtCore/QUuid>
#include <QtXml/QDomDocument>

#include "event.h"

using namespace twoDModel::constraints::details;

ConstraintsParser::ConstraintsParser(Events &events
		, Variables &variables
		, const Objects &objects
		, const utils::TimelineInterface &timeline
		, StatusReporter &status)
	: mEvents(events)
	, mVariables(variables)
	, mObjects(objects)
	, mTimeline(timeline)
	, mTriggers(mEvents, mVariables, status)
	, mConditions(mEvents, mVariables, mObjects, status)
	, mValues(mVariables, mObjects, status)
{
}

QStringList ConstraintsParser::errors() const
{
	return mErrors;
}

bool ConstraintsParser::parse(const QString &constraintsXml)
{
	if (constraintsXml.isEmpty()) {
		return true;
	}

	QDomDocument document;
	QString errorMessage;
	int errorLine = 0;
	int errorColumn = 0;
	if (!document.setContent(constraintsXml, &errorMessage, &errorLine, &errorColumn)) {
		mErrors << QString("%1:%2: %3").arg(QString::number(errorLine), QString::number(errorColumn), errorMessage);
		return false;
	}

	if (document.documentElement().tagName().toLower() != "constraints") {
		mErrors << QObject::tr("Root element must be \"constraints\" tag");
		return false;
	}

	return parse(document.documentElement());
}

bool ConstraintsParser::parse(const QDomElement &constraintsXml)
{
	mErrors.clear();
	if (constraintsXml.isNull()) {
		return true;
	}

	return parseConstraints(constraintsXml);
}

bool ConstraintsParser::parseConstraints(const QDomElement &constraints)
{
	int timelimitTagsCount = 0;
	for (QDomElement constraint = constraints.firstChildElement()
			; !constraint.isNull()
			; constraint = constraint.nextSiblingElement())
	{
		if (!addToEvents(parseConstraint(constraint))) {
			return false;
		}

		if (constraint.tagName().toLower() == "timelimit") {
			++timelimitTagsCount;
		}
	}

	if (timelimitTagsCount == 0) {
		error(QObject::tr("There must be a \"timelimit\" constraint."));
		return false;
	}

	if (timelimitTagsCount > 1) {
		error(QObject::tr("There must be only one \"timelimit\" tag."));
		return false;
	}

	return mErrors.isEmpty();
}

Event *ConstraintsParser::parseConstraint(const QDomElement &constraint)
{
	const QString name = constraint.tagName().toLower();

	if (name == "event") {
		return parseEventTag(constraint);
	}

	if (name == "constraint") {
		return parseConstraintTag(constraint);
	}

	if (name == "timelimit") {
		return parseTimeLimitTag(constraint);
	}

	if (name == "init" || name == "initialization") {
		return parseInitializationTag(constraint);
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

	const bool setUpInitially = boolAttribute(element, "settedUpInitially", false);
	const bool dropsOnFire = boolAttribute(element, "dropsOnFire", true);

	const Trigger trigger = parseTriggersAlternative(triggerTag);

	Event * const result = new Event(id(element), mConditions.constant(true), trigger, dropsOnFire, setUpInitially);

	const Condition condition = conditionName == "condition"
			? parseConditionTag(conditionTag, *result)
			: parseConditionsTag(conditionTag, *result);

	result->setCondition(condition);

	return result;
}

Event *ConstraintsParser::parseConstraintTag(const QDomElement &element)
{
	// Constraint is just an event with fail trigger.
	// Check-once constraint is an event with 0 timeout forcing to drop.

	if (!assertChildrenExactly(element, 1)) {
		return nullptr;
	}

	if (!assertAttributeNonEmpty(element, "failMessage")) {
		return nullptr;
	}

	const QString failMessage = element.attribute("failMessage");
	const Trigger trigger = mTriggers.fail(failMessage);

	const QString checkOneAttribute = element.attribute("checkOnce", "false").toLower();
	const bool checkOnce = checkOneAttribute == "true";

	Event * const result = new Event(id(element), mConditions.constant(true), trigger, true, true);

	Condition condition = parseConditionsAlternative(element.firstChildElement(), *result);

	if (checkOnce) {
		const Value timestamp = mValues.timestamp(mTimeline);
		const Condition timeout = mConditions.timerCondition(0, true, timestamp, *result);
		condition = mConditions.combined({ timeout, condition }, Glue::And);
	}

	result->setCondition(mConditions.negation(condition));
	return result;
}

Event *ConstraintsParser::parseTimeLimitTag(const QDomElement &element)
{
	if (!assertHasAttribute(element, "value")) {
		return nullptr;
	}

	// Timelimit is just an event with timeout and fail trigger.
	const int value = intAttribute(element, "value");
	if (value < 0) {
		return nullptr;
	}

	const QString timeLimitMessage = QObject::tr("Program worked for too long time");
	const Value timestamp = mValues.timestamp(mTimeline);
	Event * const event = new Event(id(element)
			, mConditions.constant(true)
			, mTriggers.fail(timeLimitMessage)
			, true
			, true);

	const Condition condition = mConditions.timerCondition(value, true, timestamp, *event);
	event->setCondition(condition);

	return event;
}

Event *ConstraintsParser::parseInitializationTag(const QDomElement &element)
{
	const Trigger hooks = parseTriggersTag(element);
	return new Event(QString(), mConditions.constant(true), hooks, true, true);
}

Condition ConstraintsParser::parseConditionsAlternative(const QDomElement &element, Event &event)
{
	const QString name = element.tagName().toLower();
	return name == "conditions"
			? parseConditionsTag(element, event)
			: name == "condition"
					? parseConditionTag(element, event)
					: parseConditionContents(element, event);
}

Condition ConstraintsParser::parseConditionsTag(const QDomElement &element, Event &event)
{
	if (!assertChildrenMoreThan(element, 0) || !assertAttributeNonEmpty(element, "glue")) {
		return mConditions.constant(true);
	}

	const QString glueAttribute = element.attribute("glue").toLower();
	Glue glue = Glue::And;
	if (glueAttribute == "and") {
		glue = Glue::And;
	} else if (glueAttribute == "or") {
		glue = Glue::Or;
	} else {
		error(QObject::tr("\"Glue\" attribute must have value \"and\" or \"or\"."));
		return mConditions.constant(true);
	}

	QList<Condition> conditions;
	for (QDomElement condition = element.firstChildElement()
			; !condition.isNull()
			; condition = condition.nextSiblingElement())
	{
		conditions << parseConditionsAlternative(condition, event);
	}

	return mConditions.combined(conditions, glue);
}

Condition ConstraintsParser::parseConditionTag(const QDomElement &element, Event &event)
{
	if (!assertChildrenExactly(element, 1)) {
		return mConditions.constant(true);
	}

	return parseConditionContents(element.firstChildElement(), event);
}

Condition ConstraintsParser::parseConditionContents(const QDomElement &element, Event &event)
{
	const QString tag = element.tagName().toLower();

	if (tag == "not") {
		return parseNegationTag(element, event);
	}

	if (tag == "equals" || tag.startsWith("notequal")
			|| tag == "greater" || tag == "less"
			|| tag == "notgreater" || tag == "notless")
	{
		return parseComparisonTag(element);
	}

	if (tag == "inside") {
		return parseInsideTag(element);
	}

	if (tag == "settedup" || tag == "dropped") {
		return parseEventSettedDroppedTag(element);
	}

	if (tag == "timer") {
		return parseTimerTag(element, event);
	}

	if (tag == "using") {
		return parseUsingTag(element, event);
	}

	error(QObject::tr("Unknown tag \"%1\".").arg(element.tagName()));
	return mConditions.constant(true);
}

Condition ConstraintsParser::parseNegationTag(const QDomElement &element, Event &event)
{
	if (!assertChildrenExactly(element, 1)) {
		return mConditions.constant(true);
	}

	return mConditions.negation(parseConditionsAlternative(element.firstChildElement(), event));
}

Condition ConstraintsParser::parseComparisonTag(const QDomElement &element)
{
	if (!assertChildrenExactly(element, 2)) {
		return mConditions.constant(true);
	}

	const QString operation = element.tagName().toLower();

	const Value leftValue = parseValue(element.firstChildElement());
	const Value rightValue = parseValue(element.firstChildElement().nextSiblingElement());

	if (operation == "equals") {
		return mConditions.equals(leftValue, rightValue);
	}

	if (operation.startsWith("notequal")) {
		return mConditions.notEqual(leftValue, rightValue);
	}

	if (operation == "greater") {
		return mConditions.greater(leftValue, rightValue);
	}

	if (operation == "less") {
		return mConditions.less(leftValue, rightValue);
	}

	if (operation == "notgreater") {
		return mConditions.notGreater(leftValue, rightValue);
	}

	return mConditions.notLess(leftValue, rightValue);
}

Condition ConstraintsParser::parseInsideTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "objectId") || !assertAttributeNonEmpty(element, "regionId")) {
		return mConditions.constant(true);
	}

	return mConditions.inside(element.attribute("objectId"), element.attribute("regionId"));
}

Condition ConstraintsParser::parseEventSettedDroppedTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "id")) {
		return mConditions.constant(true);
	}

	const QString id = element.attribute("id");
	return element.tagName().toLower() == "settedup"
			? mConditions.settedUp(id)
			: mConditions.dropped(id);
}

Condition ConstraintsParser::parseTimerTag(const QDomElement &element, Event &event)
{
	if (!assertAttributeNonEmpty(element, "timeout")) {
		return mConditions.constant(true);
	}

	const int timeout = intAttribute(element, "timeout", 0);
	const bool forceDrop = boolAttribute(element, "forceDropOnTimeout", true);
	const Value timestamp = mValues.timestamp(mTimeline);
	return mConditions.timerCondition(timeout, forceDrop, timestamp, event);
}

Condition ConstraintsParser::parseUsingTag(const QDomElement &element, Event &event)
{
	if (!assertChildrenMoreThan(element, 1)) {
		return mConditions.constant(true);
	}

	QList<Trigger> triggers;
	Condition result;
	bool resultFound = false;
	for (QDomElement trigger = element.firstChildElement()
			; !trigger.isNull()
			; trigger = trigger.nextSiblingElement())
	{
		if (trigger.tagName().toLower() == "return") {
			if (resultFound) {
				error(QObject::tr("There must be only one tag \"return\" in \"using\" expression."));
				return mConditions.constant(true);
			}

			result = parseConditionsAlternative(trigger.firstChildElement(), event);
			resultFound = true;
		} else {
			triggers << parseTriggersAlternative(trigger);
		}
	}

	if (!resultFound) {
		error(QObject::tr("There must be \"return\" tag in \"using\" expression."));
		return mConditions.constant(true);
	}

	return mConditions.usingCondition(result, mTriggers.combined(triggers));
}

Trigger ConstraintsParser::parseTriggersAlternative(const QDomElement &element)
{
	const QString name = element.tagName().toLower();
	return name == "triggers"
			? parseTriggersTag(element)
			: name == "trigger"
					? parseTriggerTag(element)
					: parseTriggerContents(element);
}

Trigger ConstraintsParser::parseTriggersTag(const QDomElement &element)
{
	if (!assertChildrenMoreThan(element, 0)) {
		return mTriggers.doNothing();
	}

	QList<Trigger> triggers;
	for (QDomElement trigger = element.firstChildElement()
			; !trigger.isNull()
			; trigger = trigger.nextSiblingElement())
	{
		triggers << parseTriggersAlternative(trigger);
	}

	return mTriggers.combined(triggers);
}

Trigger ConstraintsParser::parseTriggerTag(const QDomElement &element)
{
	if (!assertChildrenExactly(element, 1)) {
		return mTriggers.doNothing();
	}

	return parseTriggerContents(element.firstChildElement());
}

Trigger ConstraintsParser::parseTriggerContents(const QDomElement &element)
{
	const QString tag = element.tagName().toLower();

	if (tag == "fail") {
		return parseFailTag(element);
	}

	if (tag == "success") {
		return parseSuccessTag(element);
	}

	if (tag == "setter") {
		return parseSetterTag(element);
	}

	if (tag == "setup" || tag == "drop") {
		return parseEventSetDropTag(element);
	}

	if (tag == "setstate") {
		return parseSetObjectStateTag(element);
	}

	error(QObject::tr("Unknown tag \"%1\".").arg(element.tagName()));
	return mTriggers.doNothing();
}

Trigger ConstraintsParser::parseFailTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "message")) {
		return mTriggers.doNothing();
	}

	return mTriggers.fail(element.attribute("message"));
}

Trigger ConstraintsParser::parseSuccessTag(const QDomElement &element)
{
	const bool deferred = boolAttribute(element, "deferred", false);
	return mTriggers.success(deferred);
}

Trigger ConstraintsParser::parseSetterTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "name") || !assertChildrenExactly(element, 1)) {
		return mTriggers.doNothing();
	}

	const QString name = element.attribute("name");
	const Value value = parseValue(element.firstChildElement());

	return mTriggers.setVariable(name, value);
}

Value ConstraintsParser::parseUnaryValueTag(const QDomElement &element)
{
	if (!assertChildrenExactly(element, 1)) {
		return mConditions.constant(true);
	}

	const QString operation = element.tagName().toLower();

	const Value value = parseValue(element.firstChildElement());

	if (operation == "minus") {
		return mValues.unaryMinus(value);
	}

	if (operation.startsWith("abs")) {
		return mValues.abs(value);
	}

	if (operation == "boundingrect") {
		return mValues.boundingRect(value);
	}

	return value;
}

Value ConstraintsParser::parseBinaryValueTag(const QDomElement &element)
{
	if (!assertChildrenExactly(element, 2)) {
		return mConditions.constant(true);
	}

	const QString operation = element.tagName().toLower();

	const Value leftValue = parseValue(element.firstChildElement());
	const Value rightValue = parseValue(element.firstChildElement().nextSiblingElement());

	if (operation == "sum") {
		return mValues.sum(leftValue, rightValue);
	}

	if (operation.startsWith("difference")) {
		return mValues.difference(leftValue, rightValue);
	}

	if (operation == "min") {
		return mValues.min(leftValue, rightValue);
	}

	if (operation == "max") {
		return mValues.max(leftValue, rightValue);
	}

	if (operation == "distance") {
		return mValues.distance(leftValue, rightValue);
	}

	return mValues.sum(leftValue, rightValue);
}

Trigger ConstraintsParser::parseEventSetDropTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "id")) {
		return mTriggers.doNothing();
	}

	const QString id = element.attribute("id");
	return element.tagName().toLower() == "setup"
			? mTriggers.setUpEvent(id)
			: mTriggers.dropEvent(id);
}

Trigger ConstraintsParser::parseSetObjectStateTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "object")
			|| !assertAttributeNonEmpty(element, "property")
			|| !assertChildrenExactly(element, 1))
	{
		return mTriggers.doNothing();
	}

	const Value object = mValues.objectState(element.attribute("object"));
	const QString property = element.attribute("property");
	const Value value = parseValue(element.firstChildElement());
	return mTriggers.setObjectState(object, property, value);
}

Value ConstraintsParser::parseValue(const QDomElement &element)
{
	const QString tag = element.tagName().toLower();

	if (tag == "bool") {
		return parseBoolTag(element);
	}

	if (tag == "int") {
		return parseIntTag(element);
	}

	if (tag == "double") {
		return parseDoubleTag(element);
	}

	if (tag == "string") {
		return parseStringTag(element);
	}

	if (tag == "variablevalue") {
		return parseVariableValueTag(element);
	}

	if (tag == "typeof") {
		return parseTypeOfTag(element);
	}

	if (tag == "objectstate") {
		return parseObjectStateTag(element);
	}

	if (tag == "minus" || tag == "abs" || tag == "boundingrect") {
		return parseUnaryValueTag(element);
	}

	if (tag == "sum" || tag == "difference" || tag == "min" || tag == "max" || tag == "distance") {
		return parseBinaryValueTag(element);
	}

	error(QObject::tr("Unknown value \"%1\".").arg(element.tagName()));
	return mValues.invalidValue();
}

Value ConstraintsParser::parseBoolTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "value")) {
		return mValues.invalidValue();
	}

	return mValues.boolValue(boolAttribute(element, "value"));
}

Value ConstraintsParser::parseIntTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "value")) {
		return mValues.invalidValue();
	}

	return mValues.intValue(intAttribute(element, "value"));
}

Value ConstraintsParser::parseDoubleTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "value")) {
		return mValues.invalidValue();
	}

	return mValues.doubleValue(doubleAttribute(element, "value"));
}

Value ConstraintsParser::parseStringTag(const QDomElement &element)
{
	if (!assertHasAttribute(element, "value")) {
		return mValues.invalidValue();
	}

	return mValues.stringValue(element.attribute("value"));
}

Value ConstraintsParser::parseVariableValueTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "name")) {
		return mValues.invalidValue();
	}

	return mValues.variableValue(element.attribute("name"));
}

Value ConstraintsParser::parseTypeOfTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "objectId")) {
		return mValues.invalidValue();
	}

	return mValues.typeOf(element.attribute("objectId"));
}

Value ConstraintsParser::parseObjectStateTag(const QDomElement &element)
{
	if (!assertAttributeNonEmpty(element, "object")) {
		return mValues.invalidValue();
	}

	return mValues.objectState(element.attribute("object"));
}

QString ConstraintsParser::id(const QDomElement &element) const
{
	const QString attribute = element.attribute("id");
	const QString id = attribute.isEmpty() ? QUuid::createUuid().toString() : attribute;
	return id;
}

int ConstraintsParser::intAttribute(const QDomElement &element, const QString &attributeName, int defaultValue)
{
	const QString attributeValue = element.attribute(attributeName);
	bool ok = false;
	const int result = attributeValue.toInt(&ok);
	if (!ok) {
		/// @todo: Make it warning
		error(QObject::tr("Invalid integer value \"%1\"").arg(attributeValue));
		return defaultValue;
	}

	return result;
}

qreal ConstraintsParser::doubleAttribute(const QDomElement &element, const QString &attributeName, qreal defaultValue)
{
	const QString attributeValue = element.attribute(attributeName);
	bool ok = false;
	const qreal result = attributeValue.toDouble(&ok);
	if (!ok) {
		/// @todo: Make it warning
		error(QObject::tr("Invalid floating point value \"%1\"").arg(attributeValue));
		return defaultValue;
	}

	return result;
}

bool ConstraintsParser::boolAttribute(const QDomElement &element, const QString &attributeName, bool defaultValue)
{
	const QString defaultString = defaultValue ? "true" : "false";
	const QString stringValue = element.attribute(attributeName, defaultString).toLower();
	if (stringValue != "true" && stringValue != "false") {
		/// @todo: Make it warning
		error(QObject::tr("Invalid boolean value \"%1\" (expected \"true\" or \"false\")")
				.arg(element.attribute(attributeName)));
		return defaultValue;
	}

	return stringValue == "true";
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
	/// @todo: Ignore comment nodes
	if (element.childNodes().count() != count) {
		return error(QObject::tr("%1 tag must have exactly %2 child tag(s)")
				.arg(element.tagName(), QString::number(count)));
	}

	return true;
}

bool ConstraintsParser::assertChildrenMoreThan(const QDomElement &element, int count)
{
	/// @todo: Ignore comment nodes
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

bool ConstraintsParser::assertAttributeNonEmpty(const QDomElement &element, const QString &attribute)
{
	if (!assertHasAttribute(element, attribute)) {
		return false;
	}

	if (element.attribute(attribute).isEmpty()) {
		error(QObject::tr("Attribute \"%1\" of the tag \"%2\" must not be empty.").arg(element.tagName(), attribute));
		return false;
	}

	return true;
}

bool ConstraintsParser::error(const QString &message)
{
	mErrors << message;
	return false;
}
