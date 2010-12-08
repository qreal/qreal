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

Message Message::patchMessage() const
{
	if ((mPerformed != qReal::actSetData) || (mDetails != "name"))
		return Message(Id(), qReal::actInvalid, QString(), QVariant(), QVariant());

	return Message(mTarget.diagramId(), qReal::ptchReplaceElement, qReal::msgAllElements, mPrevValue, mNewValue);
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

QString Message::toString() const
{
	QString message = msgOperation;
	switch (performed()) {
		case actInvalid:
			return qReal::msgInvalid;
		case actSetData:
			message += msgSetData;
			break;
		case actAddElement:
			message += msgAddElement;
			break;
		case actRemoveElement:
			message += msgRemoveElement;
			break;
		case actCreateDiagram:
			message += msgCreateDiagram;
			break;
		case actDestroyDiagram:
			message += msgDestroyDiagram;	//unused?
			break;
		case ptchReplaceElement:
			message += cmdReplaceElement;
			break;
	}

	message += "\n";
	if (target().idSize() > 1)
		message += msgTarget + target().toString() + "\n";
	if (!details().isNull())
		message += msgDetails + details() + "\n";
	if (!prevValue().isNull())
		message += msgPrevValue + Message::getDataString(prevValue()) + "\n";
	if (!newValue().isNull())
		message += msgNewValue + Message::getDataString(newValue()) + "\n";

	return message;
}

QLinkedList<Message> Message::parseLog(QString path)
{
	QFileInfo fi(path);
	QFile* file;
	if (fi.isDir()) {
		QDir dir(path);
		QStringList files = dir.entryList(QStringList(QString("*."+qReal::extensionLog)));
		if (files.length() != 1) {
			if (files.length() < 1)
				qDebug() << "Message::parseLog() error | No files in log directory.";
			else
				qDebug() << "Message::parseLog() error | Too many files in log directory.";
			return QLinkedList<Message>();
		}
		file = new QFile(path + files.first());

	} else if (fi.isFile())
		file = new QFile(path);
	else
		return QLinkedList<Message>();

	if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
		qDebug() << "Message::parseLog() error | Cannot open the file.";
	QTextStream stream(file);

	QLinkedList<Message> log;
	while (!stream.atEnd()) {
		QString string;
		while ((!stream.atEnd()) && (string.isEmpty()))
			string = stream.readLine();
		if (stream.atEnd())
			break;

		QString operation;
		if (string.startsWith(msgOperation))
			operation = string.remove(msgOperation);
		else
			qDebug() << "Message::parseLog() error | There is no operation string.";
		if ((operation == msgCreateDiagram) || (operation == msgDestroyDiagram)) {
			log.append(Message(Id(),
							(operation == msgCreateDiagram) ? actCreateDiagram : actDestroyDiagram,
								QString(), QVariant(), QVariant()));
			continue;
		}

		QString target = stream.readLine();
		if (target.startsWith(msgTarget))
			target = target.remove(msgTarget);
		else
			qDebug() << "Message::parseLog() error | There is no target string.";
		if ((operation == msgAddElement) || (operation == msgRemoveElement)) {
			log.append(Message(Id::loadFromString(target),
							(operation == msgAddElement) ? actAddElement : actRemoveElement,
								QString(), QVariant(), QVariant()));
			continue;
		}

		QString details;
		QString prevValue;
		QString newValue;

		details = stream.readLine();
		if (details.startsWith(msgDetails))
			details = details.remove(msgDetails);
		else
			qDebug() << "Message::parseLog() error | There is no details string.";

		prevValue = stream.readLine();
		if (prevValue.startsWith(msgPrevValue))
			prevValue = prevValue.remove(msgPrevValue);
		else
			qDebug() << "Message::parseLog() error | There is no previous value string.";

		newValue = stream.readLine();
		if (newValue.startsWith(msgNewValue))
			newValue = newValue.remove(msgNewValue);
		else
			qDebug() << "Message::parseLog() error | There is no new value string.";

		action performed;
		if (operation == msgSetData)
			performed = actSetData;
		else if (operation == cmdReplaceElement)
			performed = ptchReplaceElement;
		else {
			qDebug() << "Message::parseLog() error | Invalid operation.";
			continue;
		}
		log.append(Message(Id::loadFromString(target),
						performed,
							details, parseQVariant(prevValue) , parseQVariant(newValue)));
	}

	delete file;
	return log;
}

QString Message::getDataString(const QVariant data)
{
	QString output;
	QDebug qD = QDebug(&output);
	qD << data;
	output = output.trimmed();
	return output;
}

QVariant Message::parseQVariant(const QString data)
{
	QString string = data.trimmed();
	if ((!string.startsWith("QVariant(")) || (!string.endsWith(")"))) {
		qDebug() << "Message::parseQVariant() error | Illegal format.";
	} else {
		string.remove(0,9);
		string.chop(1);
	}
	int typeLen = string.indexOf(",");
	char* typeChr = new char(typeLen);
	for (int i = 0; i < typeLen; i++)
		typeChr[i] = string.at(i).toAscii();
	QVariant::Type type = QVariant::nameToType(typeChr);
	delete typeChr;

	if (type == QVariant::Invalid) {
		qDebug() << "Message::parseQVariant() error | Illegal type.";
		return QVariant();
	}

	string = string.remove(0,typeLen+1).trimmed();
	string.remove('\"');

	QVariant result = QVariant(string);
	if (result.convert(type)) {
		if (getDataString(result) == data)
			return result;
		else {
			qDebug() << "Message::parseQVariant() error | Output doesn't match to input.";
			qDebug() << "Message::parseQVariant() error | Input: " << data;
			qDebug() << "Message::parseQVariant() error | Ouput: " << getDataString(result);
			return QVariant();
		}
	}
	else {
		qDebug() << "Message::parseQVariant() error | Cannot convert QString to this type ("
				<< QVariant::typeToName(type) << ").";
		return QVariant();
	}
}
