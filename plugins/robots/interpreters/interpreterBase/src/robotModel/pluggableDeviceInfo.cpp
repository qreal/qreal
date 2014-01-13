#include <interpreterBase/robotModel/pluggableDeviceInfo.h>

using namespace interpreterBase::robotModel;

PluggableDeviceInfo::PluggableDeviceInfo(QString const &kitId, QString const &id, QString const &friendlyName)
	: mKitId(kitId)
	, mId(id)
	, mFriendlyName(friendlyName)
{
}

QString PluggableDeviceInfo::kitId() const
{
	return mKitId;
}

QString PluggableDeviceInfo::id() const
{
	return mId;
}

QString PluggableDeviceInfo::friendlyName() const
{
	return mFriendlyName;
}
