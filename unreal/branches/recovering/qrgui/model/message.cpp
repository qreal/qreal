#include "message.h"

using namespace qReal;

Message::Message(Id const target, action const performed, QString const details,
				 QVariant const prevValue, QVariant const newValue) :
	mTarget(target),
	mPerformed(performed),
	mDetails(details),
	mPrevValue(prevValue),
	mNewValue(newValue)
{
}

Id Message::target() const
{
	return mTarget;
}

action Message::performed() const
{
	return mPerformed;
}

QString Message::details() const
{
	return mDetails;
}

QVariant Message::prevValue() const
{
	return mPrevValue;
}

QVariant Message::newValue() const
{
	return mNewValue;
}

QString Message::getDataString(const QVariant data)
{
	QString output;
	QDebug qD = QDebug(&output);
	qD << data;
	return output;
}

