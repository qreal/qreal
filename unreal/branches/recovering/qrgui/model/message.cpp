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

QLinkedList<Message> Message::parseLog(QString path)
{
	QDir dir(path);
	QStringList files = dir.entryList(QStringList(QString("*.log")));
	if (files.length() != 1) {
		if (files.length() < 1)
			qDebug() << "Message::parseLog() error | No files in log directory.";
		else
			qDebug() << "Message::parseLog() error | Too many files in log directory.";
		return QLinkedList<Message>();
	}

	QFile file(path + files.first());
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		qDebug() << "Message::parseLog() error | Cannot open the file.";
	QTextStream stream(&file);

	QLinkedList<Message> log;
	while (!stream.atEnd()) {
		QString string = stream.readLine();
		if (string.isEmpty())
			continue;
		QString operation;
		if (string.contains(msgOperation))
			operation = string.remove(msgOperation);
		else
			qDebug() << "Message::parseLog() error | There is no operation string.";
		if ((operation == msgCreateDiagram) || (operation == msgDestroyDiagram)) {
			log.append(Message(Id(),
							(operation == msgCreateDiagram) ? actCreateDiagram : actDestroyDiagram,
								QString(), QVariant() ,QVariant()));
			continue;
		}

		QString target = stream.readLine();
		if (target.contains(msgTarget))
			target = target.remove(msgTarget);
		else
			qDebug() << "Message::parseLog() error | There is no target string.";
		if ((operation == msgAddElement) || (operation == msgRemoveElement)) {
			log.append(Message(Id::loadFromString(target),
							(operation == msgAddElement) ? actAddElement : actRemoveElement,
								QString(), QVariant() ,QVariant()));
			continue;
		}

		QString details;
		QString prevValue;
		QString newValue;

		details = stream.readLine();
		if (details.contains(msgDetails))
			details = details.remove(msgDetails);
		else
			qDebug() << "Message::parseLog() error | There is no details string.";

		prevValue = stream.readLine();
		if (prevValue.contains(msgPrevValue))
			prevValue = prevValue.remove(msgPrevValue);
		else
			qDebug() << "Message::parseLog() error | There is no previous value string.";

		newValue = stream.readLine();
		if (newValue.contains(msgNewValue))
			newValue = newValue.remove(msgNewValue);
		else
			qDebug() << "Message::parseLog() error | There is no new value string.";
	}

	return log;
}

QString Message::getDataString(const QVariant data)
{
	QString output;
	QDebug qD = QDebug(&output);
	qD << data;
	return output;
}

