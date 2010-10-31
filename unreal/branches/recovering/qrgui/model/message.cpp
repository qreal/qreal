#include "message.h"

#include <QDir>

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

QList<Message> Message::parseLog(QString path)
{
	QDir dir(path);
	QStringList files = dir.entryList(QStringList(QString("*.log")));
	if (files.length() != 1) {
		if (files.length() < 1)
			qDebug() << "Message::parseLog() error | No files in log directory.";
		else
			qDebug() << "Message::parseLog() error | Too many files in log directory.";
		return QList<Message>();
	}
	QFile file(files.first());

	return QList<Message>();
}

QString Message::getDataString(const QVariant data)
{
	QString output;
	QDebug qD = QDebug(&output);
	qD << data;
	return output;
}

