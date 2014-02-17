#include <interpreterBase/robotModel/deviceInfo.h>

#include <QtCore/QStringList>

#include <qrkernel/exception/exception.h>

using namespace interpreterBase::robotModel;

QMap<QString, DeviceInfo> DeviceInfo::mCreatedInfos = QMap<QString, DeviceInfo>();

DeviceInfo::DeviceInfo()
	: mDeviceType(nullptr)
{
}

DeviceInfo::DeviceInfo(QMetaObject const *deviceType, QString const &friendlyName)
	: mDeviceType(deviceType)
	, mFriendlyName(friendlyName)
{
}

bool DeviceInfo::isA(DeviceInfo const &parent) const
{
	QMetaObject const *currentParent = mDeviceType;
	while (currentParent && QString(parent.mDeviceType->className()) != QString(currentParent->className())) {
		currentParent = currentParent->superClass();
	}

	return currentParent != nullptr;
}

QString DeviceInfo::friendlyName() const
{
	return mFriendlyName;
}

bool DeviceInfo::isNull() const
{
	return mDeviceType == nullptr;
}

QString DeviceInfo::toString() const
{
	return QString(mDeviceType ? mDeviceType->className() : QString());
}

DeviceInfo DeviceInfo::fromString(QString const &string)
{
	if (string.isEmpty()) {
		return DeviceInfo();
	}

	if (!mCreatedInfos.contains(string)) {
		throw qReal::Exception(QString("QMetaObject for %1 not found").arg(string));
	}

	return mCreatedInfos[string];
}
