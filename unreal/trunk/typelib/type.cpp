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
			mName = "integer";
			break;
		case REAL:
			mDefaultValue->fromReal(0.0);
			mName = "real";
			break;
		case BOOLEAN:
			mDefaultValue->fromBoolean(false);
			mName = "boolean";
			break;
		case STRING:
			mDefaultValue->mValue.stringVal = new QString();
			mDefaultValue->fromString("");
			mName = "string";
			break;
		case ENUM:
			mDefaultValue = NULL;
			mName = "enum";
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
		if (!mDefaultValue)
			throw QString("Attempt to instantiate abstract type") + mName;
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
		else if (mType == REAL)
			var->mValue.realVal = val.toDouble();
		else if (mType == STRING)
			*(var->mValue.stringVal) = val;
		else if (mType == BOOLEAN)
		{
			if (val == "true")
				var->mValue.booleanVal = true;
			else if (val == "false")
				var->mValue.booleanVal = false;
			else
				throw "Bad cast";
		}
		else
			Q_ASSERT(0);
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
		else if (mType == REAL)
			var->mValue.realVal = val;
		else if (mType == STRING)
			var->mValue.stringVal->setNum(val);
		else if (mType == BOOLEAN)
			var->mValue.booleanVal = (val != 0);
		else
			Q_ASSERT(0);
	}

	bool QRealType::toBooleanValue(QRealValue const *var)
	{
		if (mType == INTEGER)
			return var->mValue.integerVal != 0;
		if (mType == REAL)
			throw "Bad cast";
		if (mType == STRING)
		{
			if (*(var->mValue.stringVal) == "true")
				return true;
			else if (*(var->mValue.stringVal) == "false")
				return false;
			else
				throw "Bad cast";
		}
		if (mType == BOOLEAN)
			return var->mValue.booleanVal;
		Q_ASSERT(0);
	}

	void QRealType::fromBooleanValue(QRealValue *var, bool val)
	{
		if (mType == INTEGER)
			var->mValue.integerVal = val?1:0;
		else if (mType == REAL)
			throw "Bad cast";
		else if (mType == STRING)
			*(var->mValue.stringVal) = val?"true":"false";
		else if (mType == BOOLEAN)
			var->mValue.booleanVal = val;
		else
			Q_ASSERT(0);
	}

	double QRealType::toRealValue(QRealValue const *var)
	{
		if (mType == INTEGER)
			return (double)(var->mValue.integerVal);
		if (mType == REAL)
			return var->mValue.realVal;
		if (mType == STRING)
			return var->mValue.stringVal->toDouble();
		if (mType == BOOLEAN)
			throw "Bad cast";
		Q_ASSERT(0);
	}

	void QRealType::fromRealValue(QRealValue *var, double val)
	{
		if (mType == INTEGER)
			var->mValue.integerVal = (int)val;
		else if (mType == REAL)
			var->mValue.realVal = val;
		else if (mType == STRING)
			var->mValue.stringVal->setNum(val);
		else if (mType == BOOLEAN)
			throw "Bad cast";
		else
			Q_ASSERT(0);
	}
}
