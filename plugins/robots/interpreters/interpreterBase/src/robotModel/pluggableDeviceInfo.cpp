#include <interpreterBase/robotModel/pluggableDeviceInfo.h>

using namespace interpreterBase::robotModel;

PluggableDeviceInfo::PluggableDeviceInfo(QString const &id, QString const &friendlyName)
	: mId(id)
	, mFriendlyName(friendlyName)
{
}

QString PluggableDeviceInfo::id() const
{
	return mId;
}

QString PluggableDeviceInfo::friendlyName() const
{
	return mFriendlyName;
}
