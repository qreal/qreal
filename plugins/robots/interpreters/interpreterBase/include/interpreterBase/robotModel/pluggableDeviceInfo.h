#pragma once

#include <QtCore/QString>

namespace interpreterBase {
namespace robotModel {

/// Describes a type of a pluggable device, not a concrete instance of it
class PluggableDeviceInfo
{
public:
	PluggableDeviceInfo(QString const &kitId, QString const &id, QString const &friendlyName);

	QString kitId() const;
	QString id() const;
	QString friendlyName() const;

private:
	friend bool operator ==(PluggableDeviceInfo const &, PluggableDeviceInfo const &device2);
	friend bool operator !=(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2);

	QString mKitId;
	QString mId;
	QString mFriendlyName;
};

inline bool operator ==(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2)
{
	return device1.mKitId == device2.mKitId && device1.mId == device2.mId;
}

inline bool operator !=(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2)
{
	return !(device1 == device2);
}

}
}
