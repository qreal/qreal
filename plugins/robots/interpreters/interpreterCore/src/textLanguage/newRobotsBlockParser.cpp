#include "newRobotsBlockParser.h"

#include <qrtext/lua/types/integer.h>

using namespace qReal;
using namespace interpreterCore::textLanguage;
using namespace qrtext::lua;

QString const sensorVariablePerfix = QObject::tr("sensor");
QString const encoderVariablePerfix = QObject::tr("encoder");
QString const timeVariableName = QObject::tr("time");

NewRobotsBlockParser::NewRobotsBlockParser(ErrorReporterInterface * const errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, utils::ComputableNumber::IntComputer const &timeComputer)
	: qrtext::lua::LuaToolbox()
	, mRobotModelManager(robotModelManager)
	, mTimeComputer(timeComputer)
{
	setReservedVariables();
	connect(&mRobotModelManager, &interpreterBase::robotModel::RobotModelManagerInterface::robotModelChanged
			, this, &NewRobotsBlockParser::setReservedVariables);
}

void NewRobotsBlockParser::setReservedVariables()
{
	interpret<int>(Id(), "", "pi = 3.14159265");

	addIntrinsicFunction("time", new types::Integer(), {}, [this](QList<QVariant> params) { return mTimeComputer(); });

	for (interpreterBase::robotModel::PortInfo const &port : mRobotModelManager.model().availablePorts()) {
		QString const variable = port.reservedVariable();
		if (!variable.isEmpty()) {
			addIntrinsicFunction(variable, new types::Integer(), {}, [this](QList<QVariant> params) { return 0; });
		}
	}
}
