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
			mHasParseErrors = false; /*С‡С‚РѕР±С‹ РЅРµ РїРѕР»СѓС‡РёС‚СЊ Р»РёС€РЅРёС… РѕС€РёР±РѕРє, Р±СѓРґРµРј РЅРµР·Р°РІРёСЃРёРјРѕ РѕР±СЂР°Р±Р°С‚С‹РІР°С‚СЊ РїРµСЂРµРјРµРЅРЅС‹Рµ*/
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
	QString rus = QString::fromUtf8("РђР°Р‘Р±Р’РІР“РіР”РґР•РµРЃС‘Р–Р¶Р—Р·РРёР™Р№РљРєР›Р»РњРјРќРЅРћРѕРџРїР СЂРЎСЃРўС‚РЈСѓР¤С„РҐС…Р¦С†Р§С‡РЁС€Р©С‰Р¬СЊР«С‹Р™Р№Р­СЌР®СЋРЇСЏ");
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
