/* Copyright 2017 QReal Research Group
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

#include "communicator/httpCommunicator.h"

#include <QtCore/QFileInfo>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <qrkernel/settingsManager.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <pioneerKit/constants.h>

using namespace pioneer;
using namespace pioneer::lua;
using namespace qReal;

/// API part in URLs for HTTP requests.
const QString apiLevel = "v0.1";

/// Timeout for HTTP reply (in milliseconds).
const int timeout = 3000;

HttpCommunicator::HttpCommunicator(qReal::ErrorReporterInterface &errorReporter)
	: mNetworkManager(new QNetworkAccessManager)
	, mErrorReporter(errorReporter)
	, mRequestTimeoutTimer(new QTimer)
{
	connect(mNetworkManager.data(), &QNetworkAccessManager::finished, this, &HttpCommunicator::onPostRequestFinished);
	connect(mRequestTimeoutTimer.data(), &QTimer::timeout, this, &HttpCommunicator::onTimeout);
	mRequestTimeoutTimer->setInterval(timeout);
	mRequestTimeoutTimer->setSingleShot(true);
}

HttpCommunicator::~HttpCommunicator()
{
	disconnect(mNetworkManager.data(), &QNetworkAccessManager::finished
			, this, &HttpCommunicator::onPostRequestFinished);
	disconnect(mRequestTimeoutTimer.data(), &QTimer::timeout
			, this, &HttpCommunicator::onTimeout);
}

void HttpCommunicator::uploadProgram(const QFileInfo &program)
{
	const QString ip = SettingsManager::value(settings::pioneerBaseStationIP).toString();
	if (ip.isEmpty()) {
		mErrorReporter.addError(tr("Pioneer base station IP address is not set. It can be set in Settings window."));
		return;
	}

	const QString port = SettingsManager::value(settings::pioneerBaseStationPort).toString();
	if (port.isEmpty()) {
		mErrorReporter.addError(tr("Pioneer base station port is not set. It can be set in Settings window."));
		return;
	}

	QFile programFile(program.canonicalFilePath());
	if (!programFile.open(QIODevice::ReadOnly)) {
		mErrorReporter.addError(tr("Generation failed, upload aborted."));
		emit uploadCompleted(false);
		return;
	}

	QByteArray programData = programFile.readAll();
	programFile.close();

	if (programData.isEmpty()) {
		mErrorReporter.addError(tr("Generation failed, upload aborted."));
		emit uploadCompleted(false);
		return;
	}

	const QString url = QString("http://%1:%2/pioneer/%3/upload").arg(ip).arg(port).arg(apiLevel);
	mErrorReporter.addInformation(QString(tr("Uploading to: %1, please wait...")).arg(url));

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");

	mCurrentReply= mNetworkManager->post(request, programData);
	mRequestTimeoutTimer->start();
}

void HttpCommunicator::startProgram()
{
	const QString ip = SettingsManager::value(settings::pioneerBaseStationIP).toString();
	if (ip.isEmpty()) {
		mErrorReporter.addError(tr("Pioneer base station IP address is not set. It can be set in Settings window."));
		return;
	}

	const QString port = SettingsManager::value(settings::pioneerBaseStationPort).toString();
	if (port.isEmpty()) {
		mErrorReporter.addError(tr("Pioneer base station port is not set. It can be set in Settings window."));
		return;
	}

	const QString url = QString("http://%1:%2/pioneer/%3/start").arg(ip).arg(port).arg(apiLevel);
	mErrorReporter.addInformation(QString(tr("Starting program. Senging request to: %1, please wait...")).arg(url));
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");

	mCurrentReply = mNetworkManager->post(request, QByteArray());
	mRequestTimeoutTimer->start();
}

void HttpCommunicator::stopProgram()
{
	mErrorReporter.addError(tr("Stopping program is not supported for HTTP communication mode."));
	emit stopCompleted(false);
}

void HttpCommunicator::onPostRequestFinished(QNetworkReply *reply)
{
	mRequestTimeoutTimer->stop();

	if (reply->url().toString().endsWith("/upload")) {
		if (reply->error() != QNetworkReply::NoError) {
			mErrorReporter.addError(reply->errorString());
			emit uploadCompleted(false);
		} else {
			mErrorReporter.addInformation(tr("Uploading finished."));
			emit uploadCompleted(true);
		}
	} else if (reply->url().toString().endsWith("/start")) {
		if (reply->error() != QNetworkReply::NoError) {
			mErrorReporter.addError(reply->errorString());
			emit startCompleted(false);
		} else {
			mErrorReporter.addInformation(tr("Start finished."));
			emit startCompleted(true);
		}
	}

	reply->deleteLater();
}

void HttpCommunicator::onTimeout()
{
	if (mCurrentReply) {
		mErrorReporter.addError(tr("Pioneer base station took too long to respond. Request aborted."));
		mCurrentReply->abort();
		mCurrentReply = nullptr;
	}
}
