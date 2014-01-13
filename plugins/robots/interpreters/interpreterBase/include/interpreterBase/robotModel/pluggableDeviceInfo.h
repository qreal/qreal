#pragma once

#include <QtCore/QString>

namespace interpreterBase {
namespace robotModel {

/// Describes a type of a pluggable device, not a concrete instance of it
class PluggableDeviceInfo
{
public:
	PluggableDeviceInfo(QString const &id, QString const &friendlyName);

	QString id() const;

	QString friendlyName() const;

private:
	QString mId;
	QString mFriendlyName;
};

inline bool operator ==(PluggableDeviceInfo const &left, PluggableDeviceInfo const &right)
{
	return left.id() == right.id()
			&& left.friendlyName() == right.friendlyName()
			;
}

}
}
