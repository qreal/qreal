#include <QDebug>
#include <typelib.h>

using namespace qRealType;

int main()
{
	QRealTypeFactory::init();

	// String variable
	qDebug() << "Testing string variable";
	QRealValue *testvar = QRealTypeFactory::getTypeByName("string")->newValue();

	testvar->fromString("Domine, Domine salvum fac regem Demetrium Moscoviae");
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	//qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	testvar->fromInteger(1605);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
//	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	testvar->fromReal(16.05);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
//	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	testvar->fromBoolean(true);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	delete testvar;

	// Integer variable
	qDebug() << "Testing integer variable";
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

	testvar->fromReal(16.05);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	testvar->fromBoolean(true);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	delete testvar;

	// Real variable
	qDebug() << "Testing real variable";
	testvar = QRealTypeFactory::getTypeByName("real")->newValue();
	testvar->fromString("16.05");
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
//	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	testvar->fromInteger(13423);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
//	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

	testvar->fromReal(16.05);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
//	qDebug() << testvar->toBoolean();
	qDebug() << testvar->toReal();

//	testvar->fromBoolean(true);
//	qDebug() << testvar->toString();
//	qDebug() << testvar->toInteger();
//	qDebug() << testvar->toBoolean();
//	qDebug() << testvar->toReal();

	delete testvar;

	// Boolean variable
	qDebug() << "Testing boolean variable";
	testvar = QRealTypeFactory::getTypeByName("boolean")->newValue();
	testvar->fromString("false");
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
//	qDebug() << testvar->toReal();

	testvar->fromInteger(13);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
//	qDebug() << testvar->toReal();

//	testvar->fromReal(16.05);
//	qDebug() << testvar->toString();
//	qDebug() << testvar->toInteger();
//	qDebug() << testvar->toBoolean();
//	qDebug() << testvar->toReal();

	testvar->fromBoolean(true);
	qDebug() << testvar->toString();
	qDebug() << testvar->toInteger();
	qDebug() << testvar->toBoolean();
//	qDebug() << testvar->toReal();

	delete testvar;

	return 0;
}
