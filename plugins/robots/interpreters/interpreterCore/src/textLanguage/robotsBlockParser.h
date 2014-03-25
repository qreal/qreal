#pragma once

#include <qrutils/expressionsParser/expressionsParser.h>
#include <qrutils/expressionsParser/computableNumber.h>
#include <interpreterBase/blocksBase/blockParserInterface.h>

namespace interpreterCore {
namespace textLanguage {

class RobotsBlockParser : public utils::ExpressionsParser, public interpreterBase::blocksBase::BlockParserInterface
{
public:
	explicit RobotsBlockParser(qReal::ErrorReporterInterface * const errorReporter
			, utils::ComputableNumber::IntComputer const &timeComputer);

	utils::Number *standartBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId);
	void functionBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId);
	void deselect();
	void robotsClearVariables();
	void setReservedVariables();

	QMap<QString, utils::Number *> const &variables() const override;

private:
	virtual bool isLetter(QChar const &symbol);

	virtual bool checkForUsingReservedVariables(QString const &nameOfVariable);

	QStringList mReservedVariables;
	utils::ComputableNumber::IntComputer const mTimeComputer;
};

}
}
