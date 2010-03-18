#include "type.h"
#include "value.h"
#include "typefactory.h"
#include <QDebug>

using namespace qRealType;

int main()
{
	QRealTypeFactory::init();
	QRealValue *ololo = QRealTypeFactory::getTypeByName("string")->newValue();

	ololo->fromString("Domine, Domine salvum fac regem Demetrium Moscoviae");
	qDebug() << ololo->toString();

	ololo->fromInteger(1605);
	qDebug() << ololo->toString();

	return 0;
}
