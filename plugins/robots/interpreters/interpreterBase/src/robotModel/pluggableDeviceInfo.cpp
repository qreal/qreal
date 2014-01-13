#include <interpreterBase/robotModel/pluggableDeviceInfo.h>

using namespace interpreterBase::robotModel;

PluggableDeviceInfo::PluggableDeviceInfo(QMetaObject const *deviceType, QString const &friendlyName)
	: mDeviceType(deviceType)
	, mFriendlyName(friendlyName)
{
}

bool PluggableDeviceInfo::isA(PluggableDeviceInfo const &parent) const
{
	QMetaObject const *currentParent = mDeviceType;
	while (currentParent && QString(parent.mDeviceType->className()) != QString(currentParent->className())) {
		currentParent = currentParent->superClass();
	}

	return currentParent != nullptr;
}

QString PluggableDeviceInfo::friendlyName() const
{
	return mFriendlyName;
}
