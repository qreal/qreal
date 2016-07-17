/* Copyright 2007-2016 QReal Research Group, CyberTech Labs Ltd.
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

#pragma once

#include <QtCore/QFuture>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

/// Mock class for ErrorReporter. If configured properly (in QrguiFacade, for example), can emit signal when
/// new error is reported.
class ErrorReporterMock : public QObject, public qReal::ErrorReporterInterface
{
	Q_OBJECT

public:
	MOCK_METHOD2(addInformation, void(const QString &message, const qReal::Id &position));
	MOCK_METHOD2(addWarning, void(const QString &message, const qReal::Id &position));
	MOCK_METHOD2(addError, void(const QString &message, const qReal::Id &position));
	MOCK_METHOD2(addCritical, void(const QString &message, const qReal::Id &position));
	MOCK_METHOD3(sendBubblingMessage, void(const QString &message, int duration, QWidget *parent));
	MOCK_METHOD2(reportOperation, void(const QFuture<void> & operation, const QString &description));

	MOCK_METHOD0(clear, void());
	MOCK_METHOD0(clearErrors, void());
	MOCK_CONST_METHOD0(wereErrors, bool());

signals:
	/// Intended to be emitted as a reaction to addError() call, but it is up to mock object configuration to do so.
	/// Use QrguiFacade for a properly configured error reporter.
	void error();
};

}
