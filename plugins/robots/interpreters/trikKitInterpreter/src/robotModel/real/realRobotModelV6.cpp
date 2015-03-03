#include "realRobotModelV6.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

using namespace trik::robotModel::real;
using namespace kitBase::robotModel;

RealRobotModelV6::RealRobotModelV6(const QString &kitId, const QString &robotId)
	: RealRobotModelBase(kitId, robotId)
{
}

QString RealRobotModelV6::name() const
{
	return "TrikRealRobotModelV6";
}

QString RealRobotModelV6::friendlyName() const
{
	return tr("Interpretation (Wi-Fi)");
}

int RealRobotModelV6::priority() const
{
	return 8;  // Right after qts generator
}
