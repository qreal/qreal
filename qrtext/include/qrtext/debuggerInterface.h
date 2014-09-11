#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace qrtext {

/// Interface for debuggers, watchers and other tools requiring direct access to text language interpterer.
class DebuggerInterface {
public:
	virtual ~DebuggerInterface() {}

	/// A list of identifiers currently known to interpreter.
	virtual QStringList identifiers() const = 0;

	/// A value of identifier with given name.
	template<typename T>
	T value(QString const &identifier) const
	{
		return value(identifier).value<T>();
	}

	/// Sets a value of given identifier in interpreter to given value.
	template<typename T>
	void setVariableValue(QString const &name, T value)
	{
		setVariableValue(name, QString("%1 = %2").arg(name).arg(value), QVariant(value));
	}

private:
	/// Non-generic variant of value(), returns identifier value as QVariant.
	virtual QVariant value(QString const &identifier) const = 0;

	/// Non-generic variant of setVariableValue(), sets value as QVariant. Also requires textual code for initialization
	/// of a variable if it is not known to an interpreter. This code will be parsed and interpreted before value will
	/// be set.
	virtual void setVariableValue(QString const &name, QString const &initCode, QVariant const &value) = 0;
};

}

