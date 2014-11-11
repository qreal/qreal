#include "realRobotModelV6.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

using namespace trikKitInterpreter::robotModel::real;
using namespace interpreterBase::robotModel;

RealRobotModelV6::RealRobotModelV6(QString const &kitId, QString const &robotId)
	: RealRobotModelBase(kitId, robotId)
{
}

QString RealRobotModelV6::name() const
{
	return "TrikRealRobotModelV6";
}

QString RealRobotModelV6::friendlyName() const
{
	return tr("Real Robot (v6)");
}
