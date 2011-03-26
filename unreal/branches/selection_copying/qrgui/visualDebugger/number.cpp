#include "number.h"

using namespace qReal;

Number::Number(QVariant n, Type t): mNumber(n), mType(t)
{
}

Number::Number() : mNumber(0), mType(Number::intType)
{
}

Number::~Number() {
}

QVariant Number::property(QString name) {
	if (name.compare("Number") == 0) {
		return mNumber;
	} else if (name.compare("Type") == 0) {
		return mType;
	}
	return QVariant();
}

void Number::setProperty(QString name, QVariant value) {
	if (name.compare("Number") == 0) {
		mNumber = value;
	} else if (name.compare("Type") == 0) {
		mType = value.toInt() ? Number::intType : Number::doubleType;
	}
}

void Number::operator+=(Number add) {
	Number::Type type = add.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = add.property("Number");
	if (mType == type) {
		switch (type) {
			case Number::intType:
				mNumber = mNumber.toInt() + val.toInt();
				break;
			case Number::doubleType:
				mNumber = mNumber.toDouble() + val.toDouble();
				break;
		}
	} else {
		mType = Number::doubleType;
		mNumber = mNumber.toDouble() + val.toDouble();
	}
}

void Number::operator-=(Number sub) {
	Number::Type type = sub.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = sub.property("Number");
	if (mType == type) {
		switch (type) {
			case Number::intType:
				mNumber = mNumber.toInt() - val.toInt();
				break;
			case Number::doubleType:
				mNumber = mNumber.toDouble() - val.toDouble();
				break;
		}
	} else {
		mType = Number::doubleType;
		mNumber = mNumber.toDouble() - val.toDouble();
	}
}

void Number::operator*=(Number mult) {
	Number::Type type = mult.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = mult.property("Number");
	if (mType == type) {
		switch (type) {
			case Number::intType:
				mNumber = mNumber.toInt() * val.toInt();
				break;
			case Number::doubleType:
				mNumber = mNumber.toDouble() * val.toDouble();
				break;
		}
	} else {
		mType = Number::doubleType;
		mNumber = mNumber.toDouble() * val.toDouble();
	}
}

void Number::operator/=(Number div) {
	Number::Type type = div.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = div.property("Number");
	if (mType == type) {
		switch (type) {
			case Number::intType:
				mNumber = mNumber.toInt() / val.toInt();
				break;
			case Number::doubleType:
				mNumber = mNumber.toDouble() / val.toDouble();
				break;
		}
	} else {
		mType = Number::doubleType;
		mNumber = mNumber.toDouble() / val.toDouble();
	}
}

Number Number::operator-() {
	switch (mType) {
		case Number::intType:
			mNumber = -mNumber.toInt();
			break;
		case Number::doubleType:
			mNumber = -mNumber.toDouble();
			break;
	}
	return *this;
}

bool Number::operator<(Number arg) {
	Number::Type type = arg.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = arg.property("Number");
	if (mType == Number::intType && type == Number::intType) {
		return mNumber.toInt() < val.toInt();
	} else if (mType == Number::intType && type == Number::doubleType) {
		return mNumber.toInt() < val.toDouble();
	} else if (mType == Number::doubleType && type == Number::intType) {
		return mNumber.toDouble() < val.toInt();
	} else {
		return mNumber.toDouble() < val.toDouble();
	}
}
bool Number::operator==(Number arg) {
	Number::Type type = arg.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = arg.property("Number");
	if (mType == Number::intType && type == Number::intType) {
		return mNumber.toInt() == val.toInt();
	} else if (mType == Number::intType && type == Number::doubleType) {
		return mNumber.toInt() == val.toDouble();
	} else if (mType == Number::doubleType && type == Number::intType) {
		return mNumber.toDouble() == val.toInt();
	} else {
		return mNumber.toDouble() == val.toDouble();
	}
}

bool Number::operator>(Number arg) {
	return !((*this) < arg || (*this) == arg);
}

bool Number::operator<=(Number arg) {
	return (*this) < arg || (*this) == arg;
}

bool Number::operator>=(Number arg) {
	return !((*this) < arg);
}

bool Number::operator!=(Number arg) {
	return !((*this) == arg);
}
