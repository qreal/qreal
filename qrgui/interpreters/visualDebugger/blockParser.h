#pragma once

#include <QMap>

#include "../../../qrutils/expressionsParser/expressionsParser.h"
#include "../mainwindow/errorReporter.h"

namespace qReal {
class BlockParser : public utils::ExpressionsParser
{

public:
	BlockParser(ErrorReporterInterface* errorReporter);

private:
	virtual void parseVarPart(QString const &stream, int& pos);
	virtual void checkForVariable(QString const &nameOfVariable, int &index);

};
}
