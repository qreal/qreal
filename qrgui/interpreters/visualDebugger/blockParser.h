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
		~BlockParser();

	private:
		virtual void parseVarPart(QString stream, int& pos);
		virtual void checkForVariable(QString nameOfVariable, int &index);

	};
}
