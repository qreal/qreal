#pragma once

#include <QtCore/QMap>

#include <qrutils/expressionsParser/textExpressionProcessorBase.h>
#include <qrutils/expressionsParser/number.h>

#include "utilsDeclSpec.h"

namespace utils {

class ROBOTS_UTILS_EXPORT TextExpressionProcessor : public utils::TextExpressionProcessorBase
{
public:
	explicit TextExpressionProcessor(QMap<QString, utils::Number> const &variables);

	QString processExpression(QString const &expression) const;

protected:
	virtual bool variableExists(QString const &variable) const;
	virtual QString value(QString const &variable) const;

private:
	QMap<QString, utils::Number> const mVariables;
};

}
