#include "exception.h"
#include <QDebug>

using namespace qReal;

Exception::Exception(QString const &message) : mMessage(message)
{
	qDebug() << "QReal exception: " << message;
}

QString Exception::message() const
{
	return mMessage;
}
