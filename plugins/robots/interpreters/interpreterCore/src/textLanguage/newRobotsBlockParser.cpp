#include "newRobotsBlockParser.h"

#include <qrtext/lua/types/integer.h>

using namespace qReal;
using namespace interpreterCore::textLanguage;
using namespace qrtext::lua;

QString const sensorVariablePerfix = QObject::tr("sensor");
QString const encoderVariablePerfix = QObject::tr("encoder");
QString const timeVariableName = QObject::tr("time");

NewRobotsBlockParser::NewRobotsBlockParser(
		interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, utils::ComputableNumber::IntComputer const &timeComputer)
	: qrtext::lua::LuaToolbox()
	, mRobotModelManager(robotModelManager)
	, mTimeComputer(timeComputer)
{
	setReservedVariables();

	addIntrinsicFunction("time", new types::Integer(), {}
			, [this](QList<QVariant> const &params) {
				Q_UNUSED(params);
				return mTimeComputer();
			});

	connect(&mRobotModelManager, &interpreterBase::robotModel::RobotModelManagerInterface::robotModelChanged
			, this, &NewRobotsBlockParser::setReservedVariables);
}

void NewRobotsBlockParser::setReservedVariables()
{
	/// @todo Remove old reserved variables for old model.

	setVariableValue("pi", 3.14159265);

	for (interpreterBase::robotModel::PortInfo const &port : mRobotModelManager.model().availablePorts()) {
		setVariableValue(port.name(), port.name());

		mSpecialVariables << port.name();

		for (QString const &alias : port.nameAliases()) {
			setVariableValue(alias, port.name());
			mSpecialVariables << alias;
		}

		if (!port.reservedVariable().isEmpty()) {
			setVariableValue(port.reservedVariable(), 0);
		}
	}
}

QStringList const &NewRobotsBlockParser::specialVariables() const
{
	return mSpecialVariables;
}
