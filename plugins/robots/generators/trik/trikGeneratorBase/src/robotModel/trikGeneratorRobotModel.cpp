#include "trikGeneratorRobotModel.h"

using namespace trik::robotModel;

TrikGeneratorRobotModel::TrikGeneratorRobotModel(const QString &kitId
		, const QString &robotId
		, const QString &name
		, const QString &friendlyName
		, int priority)
	: TrikRobotModelBase(kitId, robotId)
	, mName(name)
	, mFriendlyName(friendlyName)
	, mPriority(priority)
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

bool TrikGeneratorRobotModel::interpretedModel() const
{
	return false;
}

int TrikGeneratorRobotModel::priority() const
{
	return mPriority;
}
