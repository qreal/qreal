#include "error.h"

using namespace qReal::gui;

Error::Error(QString const &message, Severity const &severity, qReal::Id const &position)
	: mMessage(message), mTimestamp(QTime::currentTime()), mSeverity(severity), mPosition(position)
{
}

Error::Severity Error::severity() const
{
	return mSeverity;
}

QString Error::message() const
{
    return mMessage;
}

QString Error::timestamp() const
{
	return "[" + mTimestamp.toString() + "]:";
}

qReal::Id Error::position() const
{
	return mPosition;
}
