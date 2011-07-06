#include "robotsBlockParser.h"

using namespace qReal;

RobotsBlockParser::RobotsBlockParser(gui::ErrorReporter *errorReporter)
	:ExpressionsParser(errorReporter)
{
	mHasParseErrors = false;
	mErrorReporter = errorReporter;
	mCurrentId = Id::rootId();
	QString pi = "pi";
	Number value = Number(3.14, Number::doubleType);
	mVariables.insert(pi, value);
	for (int i = 1; i <= 4; ++i) {
		QString variable = QObject::tr("Sensor") + QString::number(i);
		mVariables.insert(variable, Number(0, Number::intType));
		mReservedVariables.append(variable);
	} /*добавляем число pi и имена зарезервированных переменных со значениями по умолчанию (показания датчиков) */
	for (int i = 1; i <= 4; ++i) {
		QString variable = QObject::tr("SensorRaw") + QString::number(i);
		mVariables.insert(variable, Number(0, Number::intType));
		mReservedVariables.append(variable);
	} /*разобраться, нужны сырые значения датчиков или нет */
}

RobotsBlockParser::~RobotsBlockParser() {
	/* тут непонятно, что нужно удалять */
}

Number RobotsBlockParser::standartBlockParseProcess(QString stream, int &pos, Id curId) /* тут надо решить, нужно ли одно выражение, или можно и переменные объявлять */
{
	mCurrentId = curId;

	if (isEmpty(stream, pos)) {
		error(emptyProcess);
		return Number(0, Number::intType);
	}
	QStringList exprs = stream.split(";", QString::SkipEmptyParts);
	for (int i = 0; i < (exprs.length()-1); ++i) {
		if (mHasParseErrors)
			return Number(0, Number::intType);
		int position = 0;
		QString expr = exprs[i];
		skip(expr, position);
		parseCommand(expr + ";", position);
	}
	int position = 0;
	QString valueExpression = exprs.last();
	if(!valueExpression.contains("="))
		return parseExpression(valueExpression, position);
	else {
		error(noExpression);
		return Number(0, Number::intType);
	}
}

void RobotsBlockParser::functionBlockParseProcess(QString stream, int &pos, Id curId)
{
	mCurrentId = curId;

	if (isEmpty(stream, pos))
		error(emptyProcess);

	bool hasParseErrorsFlag = false;

	QStringList exprs = stream.split(";", QString::SkipEmptyParts);
	for (int i = 0; i < exprs.length(); ++i) {
		if (mHasParseErrors) {
			mHasParseErrors = false; /*чтобы не получить лишних ошибок, будем независимо обрабатывать переменные*/
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
	QString const pi = "pi";
	Number value = Number(3.14, Number::doubleType);
	mVariables.insert(pi, value);
	for (int i = 1; i <= 4; ++i) {
		QString variable = QObject::tr("Sensor") + QString::number(i);
		mVariables.insert(variable, Number(0, Number::intType));
		mReservedVariables.append(variable);
	}
}

bool RobotsBlockParser::checkForUsingReservedVariables(QString nameOfVariable)
{
	if ((mReservedVariables.contains(nameOfVariable)) || (isFunction(nameOfVariable))) {
		mHasParseErrors = true;
		error(usingReservedVariable, "", "", nameOfVariable);
	}
	return mHasParseErrors;
}

bool RobotsBlockParser::isLetter(QChar symbol)
{
	QString rus = QString::fromUtf8("АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЬьЫыЙйЭэЮюЯя"); /* переписать эту хреноту */
	char symbolChar = symbol.toAscii();
	return (('A'<=symbolChar && symbolChar<='Z') || ('a'<=symbolChar && symbolChar<='z') || (rus.contains(symbol)));
}
