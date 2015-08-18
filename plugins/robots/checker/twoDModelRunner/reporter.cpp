/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "reporter.h"

#include <QtCore/QPointF>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <qrutils/outFile.h>

using namespace twoDModel;

Reporter::Reporter(const QString &messagesFile, const QString &trajectoryFile)
	: mMessagesFile(new utils::OutFile(messagesFile))
	, mTrajectoryFile(new utils::OutFile(trajectoryFile))
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

void Reporter::onInterpretationStart()
{
	mFirstMessage = true;
	report("[\n", mTrajectoryFile);
}

void Reporter::onInterpretationEnd()
{
	report("]\n", mTrajectoryFile);
}

void Reporter::newTrajectoryPoint(const QString &robotId, int timestamp, const QPointF &position, qreal rotation)
{
	if (!mTrajectoryFile.isNull()) {
		QJsonObject transition({
			{ "robotId", robotId }
			, { "timestamp", timestamp }
			, { "x", position.x() }
			, { "y", position.y() }
			, { "rotation", rotation }
		});

		QJsonDocument document;
		document.setObject(transition);
		report((mFirstMessage ? "" : ", ") + document.toJson(), mTrajectoryFile);
		mFirstMessage = false;
	}
}

void Reporter::newDeviceState(const QString &robotId, int timestamp, const QString &deviceType
		, const QString &devicePort, const QString &property, const QVariant &value)
{
	if (!mTrajectoryFile.isNull()) {
		QJsonObject modification({
			{ "robotId", robotId }
			, { "timestamp", timestamp }
			, { "device", deviceType }
			, { "port", devicePort }
			, { "property", property }
			, { "value", variantToJson(value) }
		});

		QJsonDocument document;
		document.setObject(modification);
		report((mFirstMessage ? "" : ", ") + document.toJson(), mTrajectoryFile);
		mFirstMessage = false;
	}
}

void Reporter::reportMessages()
{
	if (!mMessagesFile) {
		return;
	}

	QJsonArray messages;
	for (const QPair<Level, QString> &message : mMessages) {
		messages.append(QJsonObject::fromVariantMap({
			{ "level", message.first == Level::information ? "info" : "error" }
			, { "message", message.second }
		}));
	}

	QJsonDocument document;
	document.setArray(messages);
	report(document.toJson(), mMessagesFile);
}

QJsonValue Reporter::variantToJson(const QVariant &value) const
{
	if (value.canConvert<QJsonArray>()) {
		return value.value<QJsonArray>();
	} else if (value.canConvert<QJsonObject>()) {
		return value.value<QJsonObject>();
	} else {
		return QJsonValue::fromVariant(value);
	}
}

void Reporter::report(const QString &message, const QScopedPointer<utils::OutFile> &file)
{
	if (!file.isNull()) {
		(*file)() << message;
		file->flush();
	}
}
