#include <QtGlobal>
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
			mDefaultValue->mValue.stringVal = new QString();
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

	QRealValue* QRealType::newValue()
	{
		return mDefaultValue->clone();
	}

	QString QRealType::toStringValue(QRealValue const *var)
	{
		if (mType == INTEGER)
			return QString().setNum(var->mValue.integerVal);
		if (mType == REAL)
			return QString().setNum(var->mValue.realVal);
		if (mType == STRING)
			return *(var->mValue.stringVal);
		if (mType == BOOLEAN)
		{
			if (var->mValue.booleanVal)
				return QString("true");
			else
				return QString("false");
		}
		Q_ASSERT(0);
	}

	void QRealType::fromStringValue(QRealValue *var, QString const &val)
	{
		if (mType == INTEGER)
			var->mValue.integerVal = val.toInt();
		if (mType == REAL)
			var->mValue.realVal = val.toDouble();
		if (mType == STRING)
			*(var->mValue.stringVal) = val;
		if (mType == BOOLEAN)
		{
			if (val == "true")
				var->mValue.booleanVal = true;
			else if (val == "false")
				var->mValue.booleanVal = false;
			else
				throw "Bad cast";
		}
	}

	int QRealType::toIntegerValue(QRealValue const *var)
	{
		if (mType == INTEGER)
			return var->mValue.integerVal;
		if (mType == REAL)
			return (int)(var->mValue.realVal);
		if (mType == STRING)
			return var->mValue.stringVal->toInt();
		if (mType == BOOLEAN)
		{
			if (var->mValue.booleanVal)
				return 1;
			else
				return 0;
		}
		Q_ASSERT(0);
	}

	void QRealType::fromIntegerValue(QRealValue *var, int val)
	{
		if (mType == INTEGER)
			var->mValue.integerVal = val;
		if (mType == REAL)
			var->mValue.realVal = val;
		if (mType == STRING)
			var->mValue.stringVal->setNum(val);
		if (mType == BOOLEAN)
		{
			var->mValue.booleanVal = (val == 0);
		}
	}

	bool QRealType::toBooleanValue(QRealValue const *var)
	{
		// FIXME:
		return false;
	}

	void QRealType::fromBooleanValue(QRealValue *var, bool val)
	{
		// FIXME:
	}

	double QRealType::toRealValue(QRealValue const *var)
	{
		// FIXME:
		return 0.0;
	}

	void QRealType::fromRealValue(QRealValue *var, double val)
	{
		// FIXME:
	}
}
