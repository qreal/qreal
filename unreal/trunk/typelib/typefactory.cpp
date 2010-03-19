#include "typefactory.h"

using namespace qRealType;

// It is safe to keep it here. Since it is static it cannot be used outside this file.
// I hope so.
static QRealTypeFactory *gInstance;

QRealTypeFactory::QRealTypeFactory()
{
	mTypes["string"] = new QRealType(STRING);
	mTypes["boolean"] = new QRealType(BOOLEAN);
	mTypes["real"] = new QRealType(REAL);
	mTypes["integer"] = new QRealType(INTEGER);
	mTypes["enum"] = new QRealType(ENUM);
}

void QRealTypeFactory::init()
{
	gInstance = new QRealTypeFactory();
}

QRealType* QRealTypeFactory::getTypeByName(QString name)
{
	return gInstance->getTypeByName_(name);
}

QRealType* QRealTypeFactory::getTypeByName_(QString name) const
{
	if (mTypes.contains(name))
		return mTypes[name];
	return NULL;
}
