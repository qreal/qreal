#include "Hint.h"

using namespace qReal::gui;

Hint::Hint(QString const &message, qReal::Id const &position)
	: mMessage(message), mTimestamp(QTime::currentTime()), mPosition(position)
{
}

QString Hint::message() const
{
    return mMessage;
}

QString Hint::timestamp() const
{
	return "[" + mTimestamp.toString() + "]:";
}

qReal::Id Hint::position() const
{
	return mPosition;
}
