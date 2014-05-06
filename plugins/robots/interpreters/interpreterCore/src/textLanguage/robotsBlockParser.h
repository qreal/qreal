#pragma once

#include <qrutils/expressionsParser/expressionsParser.h>
#include <qrutils/expressionsParser/computableNumber.h>
#include <interpreterBase/blocksBase/blockParserInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace interpreterCore {
namespace textLanguage {

class RobotsBlockParser : public QObject
		, public utils::ExpressionsParser
		, public interpreterBase::blocksBase::BlockParserInterface
{
	Q_OBJECT

public:
	RobotsBlockParser(qReal::ErrorReporterInterface * const errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, utils::ComputableNumber::IntComputer const &timeComputer);

	utils::Number *standartBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId) override;
	void functionBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId) override;
	bool parseCondition(QString const &stream, int& pos, qReal::Id const &curId) override;

	void deselect() override;
	void robotsClearVariables();
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
