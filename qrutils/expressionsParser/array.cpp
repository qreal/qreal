#include "array.h"

using namespace utils;

Array::Array(QVector<Number> v, Type t)
{
	mValue = v;
	mType = t;
}

Array::Array()
{
}

Array::~Array()
{

}

QVariant Array::evaluate()
{
	QVector <QVariant> result;
	int length = mValue.size();
	for(int i = 0; i < length; i++)
	{
			result.append(mValue[i].property("Number"));
	}
	QVariant variantResult;
	variantResult.setValue(result);
	return variantResult;
}

QString Array::toString() const
{
	QString result= "";
	for(int i = 0; i < mValue.size()-1; i++)
	{
		result += mValue[i].toString()+", ";
	}
	if (mValue.size() >=1){
		result +=mValue[mValue.size()-1].toString();
	}
	return result;
}

void Array::writeAt(int count, int value)
{
	if (count < mValue.size()){
		mValue[count].setProperty("Number", value);
	}
}

QVariant Array::getAt(int number)
{
	return mValue[number].property("Number");
}

bool Array::checkForOutOfRange(int number)
{
	return (number < mValue.size());
}
int Array::size()
{
	return mValue.size();
}



