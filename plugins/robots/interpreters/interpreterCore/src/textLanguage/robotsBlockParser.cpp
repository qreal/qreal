#include "robotsBlockParser.h"

using namespace qReal;
using namespace interpreterCore::textLanguage;
using namespace utils;

QString const sensorVariablePerfix = QObject::tr("sensor");
QString const encoderVariablePerfix = QObject::tr("encoder");
QString const timeVariableName = QObject::tr("time");

RobotsBlockParser::RobotsBlockParser(ErrorReporterInterface * const errorReporter
		, ComputableNumber::IntComputer const &timeComputer)
	: ExpressionsParser(errorReporter)
	, mTimeComputer(timeComputer)
{
	setReservedVariables();
}

Number *RobotsBlockParser::standartBlockParseProcess(const QString &stream, int &pos, const Id &curId)
{
	mCurrentId = curId;

	if (isEmpty(stream, pos)) {
		error(emptyProcess);
		return new Number(0, Number::intType);
	}
	QStringList exprs = stream.split(";", QString::SkipEmptyParts);
	for (int i = 0; i < (exprs.length() - 1); ++i) {
		if (mHasParseErrors) {
			return new Number(0, Number::intType);
		}
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
		return new Number(0, Number::intType);
	}
}

void RobotsBlockParser::functionBlockParseProcess(const QString &stream, int &pos, const Id &curId)
{
	mCurrentId = curId;

	if (isEmpty(stream, pos)) {
		error(emptyProcess);
	}

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
	char symbolChar = symbol.toLatin1();
	return (('A'<=symbolChar && symbolChar<='Z') || ('a'<=symbolChar && symbolChar<='z') || (rus.contains(symbol)));
}

void RobotsBlockParser::setReservedVariables()
{
	QString const pi = "pi";
	Number * const piValue = new Number(3.14159265, Number::doubleType);
	mVariables.insert(pi, piValue);
	mVariables.insert(timeVariableName, new ComputableNumber(mTimeComputer));
	mReservedVariables.append(timeVariableName);

	for (int i = 1; i <= 4; ++i) {
		QString const variable = sensorVariablePerfix + QString::number(i);
		mVariables.insert(variable, new Number(0, Number::intType));
		mReservedVariables.append(variable);
	}

	for (int i = 0; i < 3; ++i) {
		QString const variable = encoderVariablePerfix + ('A' + i);
		mVariables.insert(variable, new Number(0, Number::intType));
		mReservedVariables.append(variable);
	}
}

QMap<QString, Number *> const &RobotsBlockParser::variables() const
{
	return ExpressionsParser::variables();
}

bool RobotsBlockParser::hasErrors() const
{
	return ExpressionsParser::hasErrors();
}
