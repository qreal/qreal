#include "number.h"

using namespace qReal;

number::number(QVariant n, Type t): mNumber(n), mType(t) {
}

number::number() {
	mType = Int;
	mNumber = 0;
}

number::~number() {
}

QVariant number::property(QString name) {
	if (name.compare("Number") == 0) {
		return mNumber;
	} else if (name.compare("Type") == 0) {
		return mType;
	}
	return NULL;
}

void number::setProperty(QString name, QVariant value) {
	if (name.compare("Number") == 0) {
		mNumber = value;
	} else if (name.compare("Type") == 0) {
		mType = value.toInt() ? number::Int : number::Double;
	}
}

void number::operator+=(number add) {
	number::Type t = add.property("Type").toInt() ? number::Int : number::Double;
	QVariant val = add.property("Number");
	if (mType == t) {
		switch (t) {
			case Int:
				mNumber = mNumber.toInt() + val.toInt();
				break;
			case Double:
				mNumber = mNumber.toDouble() + val.toDouble();
				break;
		}
	} else {
		mType = Double;
		mNumber = mNumber.toDouble() + val.toDouble();
	}
}

void number::operator-=(number sub) {
	number::Type t = sub.property("Type").toInt() ? number::Int : number::Double;
	QVariant val = sub.property("Number");
	if (mType == t) {
		switch (t) {
			case Int:
				mNumber = mNumber.toInt() - val.toInt();
				break;
			case Double:
				mNumber = mNumber.toDouble() - val.toDouble();
				break;
		}
	} else {
		mType = Double;
		mNumber = mNumber.toDouble() - val.toDouble();
	}
}

void number::operator*=(number mult) {
	number::Type t = mult.property("Type").toInt() ? number::Int : number::Double;
	QVariant val = mult.property("Number");
	if (mType == t) {
		switch (t) {
			case Int:
				mNumber = mNumber.toInt() * val.toInt();
				break;
			case Double:
				mNumber = mNumber.toDouble() * val.toDouble();
				break;
		}
	} else {
		mType = Double;
		mNumber = mNumber.toDouble() * val.toDouble();
	}
}

void number::operator/=(number div) {
	number::Type t = div.property("Type").toInt() ? number::Int : number::Double;
	QVariant val = div.property("Number");
	if (mType == t) {
		switch (t) {
			case Int:
				mNumber = mNumber.toInt() / val.toInt();
				break;
			case Double:
				mNumber = mNumber.toDouble() / val.toDouble();
				break;
		}
	} else {
		mType = Double;
		mNumber = mNumber.toDouble() / val.toDouble();
	}
}

number number::operator-() {
	switch (mType) {
		case Int:
			mNumber = -mNumber.toInt();
			break;
		case Double:
			mNumber = -mNumber.toDouble();
			break;
	}
	return *this;
}

bool number::operator<(number arg) {
	number::Type t = arg.property("Type").toInt() ? number::Int : number::Double;
	QVariant val = arg.property("Number");
	if (mType == number::Int && t == number::Int) {
		return mNumber.toInt() < val.toInt();
	} else if (mType == number::Int && t == number::Double) {
		return mNumber.toInt() < val.toDouble();
	} else if (mType == number::Double && t == number::Int) {
		return mNumber.toDouble() < val.toInt();
	} else {
		return mNumber.toDouble() < val.toDouble();
	}
}
bool number::operator==(number arg) {
	number::Type t = arg.property("Type").toInt() ? number::Int : number::Double;
	QVariant val = arg.property("Number");
	if (mType == number::Int && t == number::Int) {
		return mNumber.toInt() == val.toInt();
	} else if (mType == number::Int && t == number::Double) {
		return mNumber.toInt() == val.toDouble();
	} else if (mType == number::Double && t == number::Int) {
		return mNumber.toDouble() == val.toInt();
	} else {
		return mNumber.toDouble() == val.toDouble();
	}
}

bool number::operator>(number arg) {
	return !((*this)<arg || (*this)==arg);
}

bool number::operator<=(number arg) {
	return (*this)<arg || (*this)==arg;
}

bool number::operator>=(number arg) {
	return !((*this)<arg);
}

bool number::operator!=(number arg) {
	return !((*this)==arg);
}
