#pragma once

#include <qrutils/expressionsParser/expressionsParser.h>
#include <qrutils/expressionsParser/computableNumber.h>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotsBlockParser : public utils::ExpressionsParser
{
public:
	RobotsBlockParser(ErrorReporterInterface* errorReporter
			, utils::ComputableNumber::IntComputer const &timeComputer);

	utils::Number *standartBlockParseProcess(QString const &stream, int &pos, Id const &curId);  // Transfers ownership
	void functionBlockParseProcess(QString const &stream, int &pos, Id const &curId);
	void deselect();
	void robotsClearVariables();
	void setReservedVariables();

private:
	virtual bool isLetter(QChar const &symbol);

	virtual bool checkForUsingReservedVariables(QString const &nameOfVariable);

	QStringList mReservedVariables;
	utils::ComputableNumber::IntComputer const mTimeComputer;
};

}
}
}
}
