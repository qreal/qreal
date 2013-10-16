#pragma once

#include <QtCore/QStringList>

namespace utils {

/// A base class providing useful method for replacing php-style variables in
/// the given string with its values
class TextExpressionProcessorBase
{
protected:
	QString processExpression(QString const &expression) const;
	QString processExpression(QString const &expression, QStringList &metVariables) const;

	virtual bool variableExists(QString const &variable) const = 0;
	virtual QString value(QString const &variable) const = 0;
};

}
