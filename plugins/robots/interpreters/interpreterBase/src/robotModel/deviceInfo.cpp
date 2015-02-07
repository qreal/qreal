#include <interpreterBase/robotModel/deviceInfo.h>

#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>

#include <qrkernel/exception/exception.h>

using namespace interpreterBase::robotModel;

QMap<QString, DeviceInfo> DeviceInfo::mCreatedInfos = QMap<QString, DeviceInfo>();

DeviceInfo::DeviceInfo()
	: mDeviceType(nullptr)
	, mDirection(input)
{
}

DeviceInfo::DeviceInfo(const QMetaObject *deviceType, const QString &name
		, const QString &friendlyName, Direction direction)
	: mDeviceType(deviceType)
	, mName(name)
	, mFriendlyName(friendlyName)
	, mDirection(direction)
{
}

bool DeviceInfo::isA(const DeviceInfo &parent) const
{
	if (parent.isNull()) {
		return isNull();
	}

	const QMetaObject *currentParent = mDeviceType;
	while (currentParent && QString(parent.mDeviceType->className()) != QString(currentParent->className())) {
		currentParent = currentParent->superClass();
	}

	return currentParent != nullptr;
}

QString DeviceInfo::name() const
{
	return mName;
}

QString DeviceInfo::friendlyName() const
{
	return mDeviceType
			? QCoreApplication::translate(mDeviceType->className(), mFriendlyName.toLatin1())
			: QString();
}

Direction DeviceInfo::direction() const
{
	return mDirection;
}

bool DeviceInfo::isNull() const
{
	return mDeviceType == nullptr;
}

QString DeviceInfo::toString() const
{
	return QString(mDeviceType ? mDeviceType->className() : QString());
}

DeviceInfo DeviceInfo::fromString(const QString &string)
{
	if (string.isEmpty()) {
		return DeviceInfo();
	}

	if (!mCreatedInfos.contains(string)) {
		throw qReal::Exception(QString("QMetaObject for %1 not found").arg(string));
	}

	return mCreatedInfos[string];
}

QString DeviceInfo::property(const QMetaObject * const metaObject, const QString &name)
{
	if (!metaObject) {
		return QString();
	}

	// Superclass infos are have less indeces so iterating backwards for meeting device`s
	// friendly name earlier than its parent`s one.
	for (int i = metaObject->classInfoCount() - 1; i >= 0; --i) {
		const QMetaClassInfo classInfo = metaObject->classInfo(i);
		if (QString(classInfo.name()) == name) {
			return QString(classInfo.value());
		}
	}

	return QString();
}
