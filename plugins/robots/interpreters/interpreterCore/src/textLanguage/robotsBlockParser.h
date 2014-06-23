#pragma once

#include <qrutils/expressionsParser/expressionsParser.h>
#include <qrutils/expressionsParser/computableNumber.h>
#include <interpreterBase/blocksBase/blockParserInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace interpreterCore {
namespace textLanguage {

/// Parser of textual language that is used within blocks.
/// @todo Rewrite it completely.
class RobotsBlockParser : public QObject
		, public utils::ExpressionsParser
		, public interpreterBase::blocksBase::BlockParserInterface
{
	Q_OBJECT

public:
	/// Constructor
	/// @param errorReporter - where to report parser errors.
	/// @param robotModelManager - provides information about robot model.
	/// @param timeComputer - method to get time elapsed since start of interpretation, used in predefined variable.
	RobotsBlockParser(qReal::ErrorReporterInterface * const errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, utils::ComputableNumber::IntComputer const &timeComputer);

	utils::Number *standartBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId) override;
	void functionBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId) override;
	bool parseCondition(QString const &stream, int& pos, qReal::Id const &curId) override;

	void deselect() override;

	/// Clear and reinitialize all variables.
	/// @todo Strange name. Rewrite this completely.
	void setReservedVariables();

	QMap<QString, utils::Number *> const &variables() const override;
	bool hasErrors() const override;

private:
	virtual bool isLetter(QChar const &symbol);

	virtual bool checkForUsingReservedVariables(QString const &nameOfVariable);

	interpreterBase::robotModel::RobotModelManagerInterface const &mRobotModelManager;
	utils::ComputableNumber::IntComputer const mTimeComputer;
	QStringList mReservedVariables;
};

}
}
