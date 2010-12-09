#include "message.h"

#include <QDir>

using namespace qReal;

Message::Message() :
	mValid(false),
	mPerformed(actSetData)
{
}

Message::Message(Id const scene, Id const target, const action performed) :
	mScene(scene),
	mTarget(target),
	mPerformed(performed)
{
	if ((performed == actSetData) || (performed == actReplaceElement))
		mValid = false;
	else
		mValid = true;
}

Message::Message(Id const scene, Id const target, action const performed,
	QString const details, QVariant const prevValue, QVariant const newValue) :
	mValid(true),
	mScene(scene),
	mTarget(target),
	mPerformed(performed),
	mDetails(details),
	mPrevValue(prevValue),
	mNewValue(newValue)
{
	if ((performed == actAddElement) || (performed == actRemoveElement))
		mValid = false;
	else
		mValid = true;
}

Id Message::scene() const
{
	return mScene;
}

Id Message::target() const
{
	return mTarget;
}

bool Message::valid() const
{
	return mValid;
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
	QString message = msgScene + mScene.toString() + '\n';

	message += msgOperation;
	switch (performed()) {
		case actSetData:
			message += msgSetData + '\n';
			break;
		case actAddElement:
			message += msgAddElement + '\n';
			break;
		case actRemoveElement:
			message += msgRemoveElement + '\n';
			break;
		case actReplaceElement:
			message += msgReplaceElement + '\n';
			break;
	}

	if (target().idSize() > 1)
		message += msgTarget + target().toString() + '\n';
	if (!details().isNull())
		message += msgDetails + details() + '\n';
	if (!prevValue().isNull())
		message += msgPrevValue + Message::getDataString(prevValue()) + '\n';
	if (!newValue().isNull())
		message += msgNewValue + Message::getDataString(newValue()) + '\n';

	return message;
}

QLinkedList<Message> Message::parseLog(QString const path)
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

		Id scene;
		if (string.startsWith(msgScene))
			scene = Id::loadFromString(string.remove(msgScene));
		else
			qDebug() << "Message::parseLog() error | There is no scene string.";

		QString operation;
		string = stream.readLine();
		if (string.startsWith(msgOperation))
			operation = string.remove(msgOperation);
		else
			qDebug() << "Message::parseLog() error | There is no operation string.";

		Id target;
		string = stream.readLine();
		if (string.startsWith(msgTarget))
			target = Id::loadFromString(string.remove(msgTarget));
		else
			qDebug() << "Message::parseLog() error | There is no target string.";

		action performed;
		if ((operation == msgAddElement) || (operation == msgRemoveElement)) {
			performed = (operation == msgAddElement) ? actAddElement : actRemoveElement;
			log.append(Message(scene, target, performed));
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

		if (operation == msgSetData)
			performed = actSetData;
		else if (operation == msgReplaceElement)
			performed = actReplaceElement;
		else {
			qDebug() << "Message::parseLog() error | Invalid operation.";
			continue;
		}
		log.append(Message(scene, target, performed,
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

Message Message::generatePatchMessage() const
{
	if ((mPerformed != qReal::actSetData) || (mDetails != "name"))
		return Message();

	return Message(mScene, mTarget.diagramId(), qReal::actReplaceElement, qReal::msgAllElements, mPrevValue, mNewValue);
}
