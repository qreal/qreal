#include "type.h"
#include "value.h"
#include "typefactory.h"
#include <QDebug>

using namespace qRealType;

int main()
{
	QRealTypeFactory::init();

	// String variable
	QRealValue *testvar = QRealTypeFactory::getTypeByName("string")->newValue();

	testvar->fromString("Domine, Domine salvum fac regem Demetrium Moscoviae");
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	testvar->fromInteger(1605);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	delete testvar;

	// Integer variable
	testvar = QRealTypeFactory::getTypeByName("integer")->newValue();
	testvar->fromString("1605");
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	testvar->fromInteger(13423);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	delete testvar;

	return 0;
}
