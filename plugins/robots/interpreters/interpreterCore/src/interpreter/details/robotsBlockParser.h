#pragma once

#include <qrutils/expressionsParser/expressionsParser.h>

#include <interpreterBase/blocksBase/blockParserInterface.h>

namespace interpreterCore {
namespace interpreter {
namespace details {

class RobotsBlockParser : public utils::ExpressionsParser, public interpreterBase::blocksBase::BlockParserInterface
{
public:
	explicit RobotsBlockParser(qReal::ErrorReporterInterface* errorReporter);

	utils::Number standartBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId);
	void functionBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId);
	void deselect();
	void robotsClearVariables();
	void setReservedVariables();

private:
	virtual bool isLetter(QChar const &symbol);

	virtual bool checkForUsingReservedVariables(QString const &nameOfVariable);

	QStringList mReservedVariables;
};

}
}
}
