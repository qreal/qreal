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
		if (!mVariables.contains(name)) {
			// We do not mind the situation when trying to read non-declared varible
			// because really can`t manage the order in which Qt will call event checking,
			// so some variables (for example counters) can be checked before setted.
			return QVariant();
		}

		return mVariables[name];
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

Value ValuesFactory::objectState(const QString &objectId, const QString &property) const
{
	return [this, objectId, property]() {
		if (!mObjects.contains(objectId)) {
			reportError(QObject::tr("No such object: %1").arg(objectId));
			return QVariant();
		}

		QObject const *object = mObjects[objectId];
		if (!object) {
			return QVariant();
		}

		const int index = object->metaObject()->indexOfProperty(qPrintable(property));
		if (index < 0) {
			reportError(QObject::tr("Object \"%1\" has no property \"%2\"").arg(objectId, property));
			return QVariant();
		}

		return object->property(qPrintable(property));
	};
}

Value ValuesFactory::timestamp(const utils::TimelineInterface &timeline) const
{
	return [&timeline]() { return timeline.timestamp(); };
}

void ValuesFactory::reportError(const QString &message)
{
	emit mStatus.checkerError(message);
}
