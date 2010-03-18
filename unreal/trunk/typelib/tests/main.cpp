#include "type.h"
#include "value.h"
#include "typefactory.h"
#include <QDebug>

using namespace qRealType;

int main()
{
	QRealTypeFactory::init();
	QRealValue ololo = QRealTypeFactory::getTypeByName("string")->newValue();

	qDebug() << ololo.toString();
	return 0;
}
