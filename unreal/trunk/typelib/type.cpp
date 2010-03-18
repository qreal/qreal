#include "type.h"

namespace qRealType {
	QRealType::QRealType(QREAL_METATYPE type)
	{
		mType = type;
		mDefaultValue = new QRealValue(this);

		switch (mType)
		{
		case INTEGER:
			mDefaultValue->fromInteger(0);
			mName = "Integer";
			break;
		case REAL:
			mDefaultValue->fromReal(0.0);
			mName = "Real";
			break;
		case BOOLEAN:
			mDefaultValue->fromBoolean(false);
			mName = "Boolean";
			break;
		case STRING:
			mDefaultValue->fromString("");
			mName = "String";
			break;
		default:
			throw "Invalid metatype";
			break;
		}
	}

	QRealType::~QRealType()
	{
		delete mDefaultValue;
	}

	QString QRealType::toString() const
	{
		return mName;
	}

	QRealValue& QRealType::newValue()
	{
		return mDefaultValue->clone();
	}
}
