#include "reporter.h"

#include <QtCore/QPointF>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <qrutils/outFile.h>

using namespace twoDModel;

Reporter::Reporter(const QString &messagesFile, const QString &trajectoryFile)
	: mMessagesFile(utils::OutFile::openOrLogError(messagesFile))
	, mTrajectoryFile(utils::OutFile::openOrLogError(trajectoryFile))
{
}

Reporter::~Reporter()
{
}

bool Reporter::lastMessageIsError()
{
	return !mMessages.isEmpty() && mMessages.last().first == Level::error;
}

void Reporter::addInformation(const QString &message)
{
	mMessages << qMakePair(Level::information, message);
}

void Reporter::addError(const QString &message)
{
	mMessages << qMakePair(Level::error, message);
}

void Reporter::newTrajectoryPoint(const QString &robotId, int timestamp, const QPointF &position, qreal rotation)
{
	if (!mTrajectoryFile.isNull()) {
		(*mTrajectoryFile)() << QString("%1 %2 %3 %4 %5\n").arg(robotId
				, QString::number(timestamp)
				, QString::number(position.x())
				, QString::number(position.y())
				, QString::number(rotation));
		mTrajectoryFile->flush();
	}
}

void Reporter::reportMessages()
{
	if (!mMessagesFile) {
		return;
	}

	QJsonArray messages;
	for (const QPair<Level, QString> &message : mMessages) {
		messages.append(QJsonObject({
			{ "level", message.first == Level::information ? "info" : "error" }
			, { "message", message.second }
		}));
	}

	QJsonDocument document;
	document.setArray(messages);
	(*mMessagesFile)() << document.toJson();
}
