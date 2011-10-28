#pragma once

#include "../../../../qrutils/expressionsParser/expressionsParser.h"

namespace qReal {

class RobotsBlockParser : public utils::ExpressionsParser
{
public:
	explicit RobotsBlockParser(ErrorReporterInterface* errorReporter);

	utils::Number standartBlockParseProcess(QString const &stream, int &pos, Id const &curId);
	void functionBlockParseProcess(QString const &stream, int &pos, Id const &curId);
	void deselect();
	void robotsClearVariables();
	void setReservedVariables();

private:
	virtual bool isLetter(QChar const &symbol);

	virtual bool checkForUsingReservedVariables(QString const &nameOfVariable);

	QStringList mReservedVariables;
};
}
