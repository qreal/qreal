#include "valuesFactory.h"

using namespace twoDModel::constraints::details;

const QString typeOfNull = "undefined";

ValuesFactory::ValuesFactory(Variables &variables, Objects &objects)
	: mVariables(variables)
	, mObjects(objects)
{
}


Value ValuesFactory::intValue(const QString &decimalString) const
{
	bool ok;
	const int value = decimalString.toInt(&ok);
	if (!ok) {
		/// @todo: Show error;
	}

	return [value]() { return value; };
}

Value ValuesFactory::floatValue(const QString &decimalString) const
{
	bool ok;
	const qreal value = decimalString.toDouble(&ok);
	if (!ok) {
		/// @todo: Show error;
	}

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
			/// @todo: Show error;
			return QVariant();
		}

		return mVariables[name];
	};
}

Value ValuesFactory::typeOf(const QString &objectId) const
{
	return [this, objectId]() {
		if (!mObjects.contains(objectId)) {
			/// @todo: Show error;
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
			/// @todo: Show error;
			return QVariant();
		}

		QObject const *object = mObjects[objectId];
		if (!object) {
			return QVariant();
		}

		const int index = object->metaObject()->indexOfProperty(qPrintable(property));
		if (index < 0) {
			/// @todo: Show error;
			return QVariant();
		}

		return object->property(qPrintable(property));
	};
}
