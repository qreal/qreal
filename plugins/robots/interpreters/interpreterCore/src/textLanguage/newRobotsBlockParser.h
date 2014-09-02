#pragma once

#include <qrutils/expressionsParser/computableNumber.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrtext/lua/luaToolbox.h>

#include <interpreterBase/robotModel/robotModelManagerInterface.h>


namespace interpreterCore {
namespace textLanguage {

/// Parser of textual language that is used within blocks.
class NewRobotsBlockParser : public QObject, public qrtext::lua::LuaToolbox
{
	Q_OBJECT

public:
	/// Constructor
	/// @param errorReporter - where to report parser errors.
	/// @param robotModelManager - provides information about robot model.
	/// @param timeComputer - method to get time elapsed since start of interpretation, used in predefined variable.
	NewRobotsBlockParser(qReal::ErrorReporterInterface * const errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, utils::ComputableNumber::IntComputer const &timeComputer);

private slots:
	void setReservedVariables();

private:
	interpreterBase::robotModel::RobotModelManagerInterface const &mRobotModelManager;
	utils::ComputableNumber::IntComputer const mTimeComputer;
};

}
}
