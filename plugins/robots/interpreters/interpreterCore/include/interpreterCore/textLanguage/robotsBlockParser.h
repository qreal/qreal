#pragma once

#include <qrutils/expressionsParser/computableNumber.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrtext/lua/luaToolbox.h>

#include <kitBase/robotModel/robotModelManagerInterface.h>

namespace interpreterCore {
namespace textLanguage {

/// Parser of textual language that is used within blocks.
class RobotsBlockParser : public QObject, public qrtext::lua::LuaToolbox
{
	Q_OBJECT

public:
	/// Constructor
	/// @param robotModelManager - provides information about robot model.
	/// @param timeComputer - method to get time elapsed since start of interpretation, used in predefined variable.
	RobotsBlockParser(const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, const utils::ComputableNumber::IntComputer &timeComputer);

	/// Returns a list of predefined variables that should be hidden from user (sensor and port variables, basically).
	const QStringList &hiddenVariables() const;

	void clear() override;

private slots:
	void setReservedVariables();

private:
	void addIntrinsicFuctions();

	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
	const utils::ComputableNumber::IntComputer mTimeComputer;
	QStringList mHiddenVariables;
};

}
}
