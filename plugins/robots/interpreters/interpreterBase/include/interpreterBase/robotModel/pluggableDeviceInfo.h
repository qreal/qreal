#pragma once

#include <QtCore/QString>
#include <QtCore/QMetaObject>

namespace interpreterBase {
namespace robotModel {

/// Describes a type of a pluggable device, not a concrete instance of it
class PluggableDeviceInfo
{
public:
	template<typename T>
	static PluggableDeviceInfo create(QString const &friendlyName = QString())
	{
		return PluggableDeviceInfo(&T::staticMetaObject, friendlyName);
	}

	template<typename T>
	static PluggableDeviceInfo create(T const &deviceInstance)
	{
		return PluggableDeviceInfo(deviceInstance.metaObject(), QString());//deviceInstance.friendlyName());
	}

	bool isA(PluggableDeviceInfo const &parent) const;

	QString friendlyName() const;

private:
	friend bool operator ==(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2);
	friend bool operator !=(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2);

	explicit PluggableDeviceInfo(QMetaObject const *deviceType, QString const &friendlyName);

	QMetaObject const *mDeviceType;
	QString mFriendlyName;
};

inline bool operator ==(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2)
{
	return QString(device1.mDeviceType->className()) == QString(device2.mDeviceType->className());
}

inline bool operator !=(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2)
{
	return !(device1 == device2);
}

}
}
