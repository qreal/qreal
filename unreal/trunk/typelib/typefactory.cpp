#include "typefactory.h"
#include "value.h"

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

QRealType* QRealTypeFactory::getTypeByName(QString const &name)
{
	return gInstance->getTypeByName_(name);
}

QRealType* QRealTypeFactory::getTypeByName_(QString const &name) const
{
	if (mTypes.contains(name))
		return mTypes[name];
	return NULL;
}

QRealType* QRealTypeFactory::newSubType(QString const &name, QConstraintList const &constr, QRealValue const *def, QRealType *type)
{
	return gInstance->newSubType_(name, constr, def, type);
}

QRealType* QRealTypeFactory::newSubType_(QString const &name, QConstraintList const &constr, QRealValue const *def, QRealType *type)
{
	if (mTypes.contains(name))
		throw "Type already exists";
	QRealType *newtype = type->clone();
	newtype->mConstraints << constr;
	// TODO: Check default value against new constraints
	delete newtype->mDefaultValue;
	newtype->mDefaultValue = def->clone();
	mTypes[name] = newtype;
	return newtype;
}
