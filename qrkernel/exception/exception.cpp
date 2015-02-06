#include "exception.h"

#include <QtCore/QDebug>

using namespace qReal;

Exception::Exception(const QString &message)
		: mMessage(message)
{
	qDebug() << "QReal exception: " << message;
}

QString Exception::message() const
{
	return mMessage;
}
