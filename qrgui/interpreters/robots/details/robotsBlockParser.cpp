#include "robotsBlockParser.h"

using namespace qReal;

RobotsBlockParser::RobotsBlockParser(gui::ErrorReporter *errorReporter)
	: ExpressionsParser(errorReporter)
{
	setReservedVariables();
}

Number RobotsBlockParser::standartBlockParseProcess(const QString &stream, int &pos, const Id &curId) /* тут надо решить, нужно ли одно выражение, или можно и переменные объявлять */
{
	mCurrentId = curId;

	if (isEmpty(stream, pos)) {
		error(emptyProcess);
		return Number(0, Number::intType);
	}

	QStringList expressions = stream.split(";", QString::SkipEmptyParts);
	QString expression = expressions.last();
	expressions.removeLast();
	if (!(expressions.isEmpty())) {
		error(incorrectVariableDeclaration);
	}
	skip(expression, pos);
	if (!(expression.isEmpty())) {
		if (pos == expression.size()){
			error(noExpression);
			return Number(0, Number::intType);
		}
		Number result = parseExpression(expression, pos);
		skip(expression, pos);
		if (pos != (expression.size())) {
			error(unexpectedSymbolAfterTheEndOfExpression);
			return Number(0, Number::intType);
		}
		else
			return result;
	}
	else {
		error(noExpression);
		return Number(0, Number::intType);
	}
}

void RobotsBlockParser::functionBlockParseProcess(const QString &stream, int &pos, const Id &curId)
{
	parseProcess(stream, pos, curId);
}

void RobotsBlockParser::deselect()
{
	mHasParseErrors = false;
}

void RobotsBlockParser::robotsClearVariables()
{
	mVariables.clear();
	setReservedVariables();
}

bool RobotsBlockParser::checkForUsingReservedVariables(const QString &nameOfVariable)
{
	if ((mReservedVariables.contains(nameOfVariable)) || (isFunction(nameOfVariable))) {
		mHasParseErrors = true;
		error(usingReservedVariable, "", "", nameOfVariable);
	}
	return mHasParseErrors;
}

bool RobotsBlockParser::isLetter(const QChar &symbol)
{
	QString rus = QString::fromUtf8("АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЬьЫыЙйЭэЮюЯя");
	char symbolChar = symbol.toAscii();
	return (('A'<=symbolChar && symbolChar<='Z') || ('a'<=symbolChar && symbolChar<='z') || (rus.contains(symbol)));
}

void RobotsBlockParser::setReservedVariables()
{
	QString const pi = "pi";
	Number value = Number(3.14, Number::doubleType);
	mVariables.insert(pi, value);
	for (int i = 1; i <= 4; ++i) {
		QString variable = QObject::tr("Sensor") + QString::number(i);
		mVariables.insert(variable, Number(0, Number::intType));
		mReservedVariables.append(variable);
	}
}
