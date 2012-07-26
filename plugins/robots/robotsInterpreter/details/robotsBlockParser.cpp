#include "robotsBlockParser.h"

using namespace qReal;
using namespace utils;

RobotsBlockParser::RobotsBlockParser(ErrorReporterInterface *errorReporter)
	: ExpressionsParser(errorReporter)
{
	setReservedVariables();
}

Number RobotsBlockParser::standartBlockParseProcess(const QString &stream, int &pos, const Id &curId)
{
	mCurrentId = curId;

	if (isEmpty(stream, pos)) {
		error(emptyProcess);
		return Number(0, Number::intType);
	}
	QStringList exprs = stream.split(";", QString::SkipEmptyParts);
	for (int i = 0; i < (exprs.length() - 1); ++i) {
		if (mHasParseErrors)
			return Number(0, Number::intType);
		int position = 0;
		QString expr = exprs[i];
		skip(expr, position);
		parseCommand(expr + ";", position);
	}
	int position = 0;
	QString valueExpression = exprs.last();
	if (!valueExpression.contains("="))
		return parseExpression(valueExpression, position);
	else {
		error(noExpression);
		return Number(0, Number::intType);
	}
}

void RobotsBlockParser::functionBlockParseProcess(const QString &stream, int &pos, const Id &curId)
{
	mCurrentId = curId;

	if (isEmpty(stream, pos))
		error(emptyProcess);

	bool hasParseErrorsFlag = false;

	QStringList exprs = stream.split(";", QString::SkipEmptyParts);
	for (int i = 0; i < exprs.length(); ++i) {
		if (mHasParseErrors) {
			mHasParseErrors = false; /*Ð¡â€¡Ð¡â€šÐ Ñ•Ð Â±Ð¡â€¹ Ð Ð…Ð Âµ Ð Ñ—Ð Ñ•Ð Â»Ð¡Ñ“Ð¡â€¡Ð Ñ‘Ð¡â€šÐ¡ÐŠ Ð Â»Ð Ñ‘Ð¡â‚¬Ð Ð…Ð Ñ‘Ð¡â€¦ Ð Ñ•Ð¡â‚¬Ð Ñ‘Ð Â±Ð Ñ•Ð Ñ”, Ð Â±Ð¡Ñ“Ð Ò‘Ð ÂµÐ Ñ˜ Ð Ð…Ð ÂµÐ Â·Ð Â°Ð Ð†Ð Ñ‘Ð¡ÐƒÐ Ñ‘Ð Ñ˜Ð Ñ• Ð Ñ•Ð Â±Ð¡Ð‚Ð Â°Ð Â±Ð Â°Ð¡â€šÐ¡â€¹Ð Ð†Ð Â°Ð¡â€šÐ¡ÐŠ Ð Ñ—Ð ÂµÐ¡Ð‚Ð ÂµÐ Ñ˜Ð ÂµÐ Ð…Ð Ð…Ð¡â€¹Ð Âµ*/
			hasParseErrorsFlag = true;
		}
		int position = 0;
		QString expr = exprs[i];
		skip(expr, position);
		parseCommand(expr + ";", position);
	}
	mHasParseErrors = hasParseErrorsFlag;
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
	QString rus = QString::fromUtf8("Ð Ñ’Ð Â°Ð â€˜Ð Â±Ð â€™Ð Ð†Ð â€œÐ Ñ–Ð â€Ð Ò‘Ð â€¢Ð ÂµÐ ÐƒÐ¡â€˜Ð â€“Ð Â¶Ð â€”Ð Â·Ð Â˜Ð Ñ‘Ð â„¢Ð â„–Ð Ñ™Ð Ñ”Ð â€ºÐ Â»Ð ÑšÐ Ñ˜Ð ÑœÐ Ð…Ð Ñ›Ð Ñ•Ð ÑŸÐ Ñ—Ð  Ð¡Ð‚Ð ÐŽÐ¡ÐƒÐ ÑžÐ¡â€šÐ ÐˆÐ¡Ñ“Ð Â¤Ð¡â€žÐ ÒÐ¡â€¦Ð Â¦Ð¡â€ Ð Â§Ð¡â€¡Ð ÐÐ¡â‚¬Ð Â©Ð¡â€°Ð Â¬Ð¡ÐŠÐ Â«Ð¡â€¹Ð â„¢Ð â„–Ð Â­Ð¡ÐŒÐ Â®Ð¡Ð‹Ð Ð‡Ð¡Ð");
	char symbolChar = symbol.toAscii();
	return (('A'<=symbolChar && symbolChar<='Z') || ('a'<=symbolChar && symbolChar<='z') || (rus.contains(symbol)));
}

void RobotsBlockParser::setReservedVariables()
{
	QString const pi = "pi";
	Number value = Number(3.14, Number::doubleType);
	mVariables.insert(pi, value);
	for (int i = 1; i <= 4; ++i) {
		QString variable = "Sensor" + QString::number(i);
		mVariables.insert(variable, Number(0, Number::intType));
		mReservedVariables.append(variable);
	}
}
