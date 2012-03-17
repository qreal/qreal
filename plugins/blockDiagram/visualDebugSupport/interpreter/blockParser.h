#pragma once

#include <QMap>

#include "../../../qrutils/expressionsParser/expressionsParser.h"
#include "../../../qrgui/mainwindow/errorReporter.h"

namespace qReal {

///Block diagram action process field parser and interpreter
class BlockParser : public QObject, public utils::ExpressionsParser
{

public:
	BlockParser(ErrorReporterInterface* errorReporter);

private:

	///Parse from stream declaration of variables and calcule its values
	virtual void parseVarPart(QString const &stream, int &pos);
	
	///Checking for the existence of variable declaration
	virtual void checkForVariable(QString const &nameOfVariable, int &index);

};

}
