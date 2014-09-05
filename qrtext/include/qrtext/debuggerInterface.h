#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace qrtext {

/// Interface for debuggers, watchers and other tools requiring direct access to text language interpterer.
class DebuggerInterface {
public:
	virtual ~DebuggerInterface() {}

	virtual QStringList identifiers() const = 0;

	template<typename T>
	T value(QString const &identifier) const
	{
		return value(identifier).value<T>();
	}

	template<typename T>
	void setVariableValue(QString const &name, T value)
	{
		setVariableValue(name, QString("%1 = %2").arg(name).arg(value), QVariant(value));
	}

private:
	virtual QVariant value(QString const &identifier) const = 0;

	virtual void setVariableValue(QString const &name, QString const &initCode, QVariant const &value) = 0;
};

}

