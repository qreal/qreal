#include "trikGeneratorRobotModel.h"

using namespace trik::robotModel;

TrikGeneratorRobotModel::TrikGeneratorRobotModel(const QString &kitId
		, const QString &robotId
		, const QString &name
		, const QString &friendlyName)
	: TrikRobotModelBase(kitId, robotId)
	, mName(name)
	, mFriendlyName(friendlyName)
{
}

QString TrikGeneratorRobotModel::name() const
{
	return mName;
}

QString TrikGeneratorRobotModel::friendlyName() const
{
	return mFriendlyName;
}

bool TrikGeneratorRobotModel::needsConnection() const
{
	return false;
}
