#pragma once

#include <QtCore/QStringList>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// A base class providing useful method for replacing php-style variables in
/// the given string with its values
class QRUTILS_EXPORT TextExpressionProcessorBase
{
protected:
	QString processExpression(QString const &expression) const;
	QString processExpression(QString const &expression, QStringList &metVariables) const;

	virtual bool variableExists(QString const &variable) const = 0;
	virtual QString value(QString const &variable, int index) const = 0;
};

}
