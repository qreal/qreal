#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace qReal {

/// Interface for collecting user experience information
/// about setting changes, click coordinates, diagram element creation
/// and others.
class UXInfoInterface
{
public:
	virtual ~UXInfoInterface() {}

	/// Recording change of settings.
	/// @param name Settings name.
	/// @param oldValue old value of the settings.
	/// @param newValue new value of the settings.
	virtual void reportSettingsChanges(QString const &name, QVariant const &oldValue, QVariant const &newValue) = 0;
};
}
