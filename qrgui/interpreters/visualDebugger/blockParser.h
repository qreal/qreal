#pragma once

#include <QMap>

#include "../expressionsParser/expressionsParser.h"
#include "number.h"
#include "../mainwindow/errorReporter.h"
#include "propertyeditorproxymodel.h"

namespace qReal {
class BlockParser : public ExpressionsParser
{

public:
	BlockParser(gui::ErrorReporter* errorReporter);

private:
	virtual void parseVarPart(QString const &stream, int& pos);
	virtual void checkForVariable(QString const &nameOfVariable, int &index);

};
}
