#include "error.h"

Error::Error(QString const &message, Severity const &severity, qReal::Id const &position)
	: mMessage(message), mSeverity(severity), mPosition(position)
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

qReal::Id Error::position() const
{
	return mPosition;
}
