#pragma once

#include "../../expressionsParser/expressionsParser.h"

namespace qReal {
class RobotsBlockParser : public ExpressionsParser
{
public:
	explicit RobotsBlockParser(gui::ErrorReporter* errorReporter);
	~RobotsBlockParser();

	Number standartBlockParseProcess(QString stream, int& pos, Id curId);
	void functionBlockParseProcess(QString stream, int &pos, Id curId);
	void deselect();
	void robotsClearVariables();

private:
	virtual bool isLetter(QChar symbol);

	virtual bool checkForUsingReservedVariables(QString nameOfVariable);

	QStringList mReservedVariables;
};
}
