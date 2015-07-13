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

#include "valuesFactory.h"

#include <QtCore/QRect>

#include <utils/timelineInterface.h>

using namespace twoDModel::constraints::details;

const QString typeOfNull = "undefined";

ValuesFactory::ValuesFactory(Variables &variables, const Objects &objects, StatusReporter &status)
	: mVariables(variables)
	, mObjects(objects)
	, mStatus(status)
{
}

Value ValuesFactory::invalidValue() const
{
	return []() { return QVariant(); };
}

Value ValuesFactory::boolValue(bool value) const
{
	return [value]() { return value; };
}

Value ValuesFactory::intValue(int value) const
{
	return [value]() { return value; };
}

Value ValuesFactory::doubleValue(qreal value) const
{
	return [value]() { return value; };
}

Value ValuesFactory::stringValue(const QString &value) const
{
	return [value]() { return value; };
}

Value ValuesFactory::variableValue(const QString &name) const
{
	return [this, name]() {
		const QStringList parts = name.split('.');
		if (parts.isEmpty()) {
			reportError(QObject::tr("Requesting variable value with empty name"));
			return QVariant();
		}

		if (!mVariables.contains(parts.first())) {
			// We do not mind the situation when trying to read non-declared varible
			// because really can`t manage the order in which Qt will call event checking,
			// so some variables (for example counters) can be checked before setted.
			return QVariant();
		}

		return propertyChain(mVariables[parts.first()], parts.mid(1), parts.first());
	};
}

Value ValuesFactory::typeOf(const QString &objectId) const
{
	return [this, objectId]() {
		if (!mObjects.contains(objectId)) {
			return typeOfNull;
		}

		QObject const *object = mObjects[objectId];
		return object == nullptr ? typeOfNull : object->metaObject()->className();
	};
}

Value ValuesFactory::objectState(const QString &path) const
{
	return [this, path]() {
		const QStringList parts = path.split('.', QString::SkipEmptyParts);
		if (parts.isEmpty()) {
			reportError(QObject::tr("Object path is empty!"));
			return QVariant();
		}

		QString objectId = parts.first();
		if (!mObjects.contains(objectId)) {
			reportError(QObject::tr("No such object: %1").arg(objectId));
			return QVariant();
		}

		int lastObjectPart = 1;
		while (lastObjectPart < parts.count() && mObjects.contains(objectId + "." + parts[lastObjectPart])) {
			objectId += "." + parts[lastObjectPart];
			++lastObjectPart;
		}

		return propertyChain(QVariant::fromValue<QObject *>(mObjects[objectId]), parts.mid(lastObjectPart), objectId);
	};
}

Value ValuesFactory::timestamp(const utils::TimelineInterface &timeline) const
{
	return [&timeline]() { return timeline.timestamp(); };
}

Value ValuesFactory::unaryMinus(const Value &value) const
{
	return [value]() { return -value().toInt(); };
}

Value ValuesFactory::abs(const Value &value) const
{
	return [value]() { return qAbs(value().toInt()); };
}

Value ValuesFactory::sum(const Value &left, const Value &right) const
{
	return [left, right]() { return left().toInt() + right().toInt(); };
}

Value ValuesFactory::difference(const Value &left, const Value &right) const
{
	return [left, right]() { return left().toInt() - right().toInt(); };
}

Value ValuesFactory::max(const Value &left, const Value &right) const
{
	return [left, right]() { return qMax(left().toInt(), right().toInt()); };
}

Value ValuesFactory::min(const Value &left, const Value &right) const
{
	return [left, right]() { return qMin(left().toInt(), right().toInt()); };
}

QVariant ValuesFactory::propertyChain(const QVariant &value
		, const QStringList &propertyChain, const QString &objectAlias) const
{
	QVariant currentValue = value;
	QString currentObjectAlias = objectAlias;
	for (const QString &property : propertyChain) {
		currentValue = propertyOf(currentValue, property, currentObjectAlias);
		currentObjectAlias += "." + property;
		if (!currentValue.isValid()) {
			return QVariant();
		}
	}

	return currentValue;
}

QVariant ValuesFactory::propertyOf(const QVariant &value, const QString &property, const QString &objectAlias) const
{
	bool ok;
	QVariant result;

	// Here must be enumerated all types whoose properties we can obtain.
	if (value.canConvert<QObject *>()) {
		result = propertyOf(value.value<QObject *>(), property, &ok);
	} else if (value.canConvert<QRect>()) {
		result = propertyOf(value.value<QRect>(), property, &ok);
	} else {
		reportError(QObject::tr("Unknown type of object \"%1\"").arg(objectAlias));
		return QVariant();
	}

	if (!ok) {
		reportError(QObject::tr("Object \"%1\" has no property \"%2\"").arg(objectAlias, property));
		return QVariant();
	}

	return result;
}

QVariant ValuesFactory::propertyOf(const QObject *object, const QString &property, bool *ok) const
{
	ok && (*ok = true);
	if (!object) {
		return QVariant();
	}

	const int index = object->metaObject()->indexOfProperty(qPrintable(property));
	if (index < 0) {
		ok && (*ok = false);
		return QVariant();
	}

	return object->property(qPrintable(property));
}

QVariant ValuesFactory::propertyOf(const QRect &rect, const QString &property, bool *ok) const
{
	ok && (*ok = true);
	if (property == "x") {
		return rect.x();
	}

	if (property == "y") {
		return rect.y();
	}

	if (property == "width") {
		return rect.width();
	}

	if (property == "height") {
		return rect.height();
	}

	ok && (*ok = false);
	return QVariant();
}

void ValuesFactory::reportError(const QString &message) const
{
	emit mStatus.checkerError(message);
}
