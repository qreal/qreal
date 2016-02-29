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

#pragma once

#include <QtCore/QFuture>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class ErrorReporterMock : public qReal::ErrorReporterInterface {
public:
	MOCK_METHOD2(addInformation, void(const QString &message, const qReal::Id &position));
	MOCK_METHOD2(addWarning, void(const QString &message, const qReal::Id &position));
	MOCK_METHOD2(addError, void(const QString &message, const qReal::Id &position));
	MOCK_METHOD2(addCritical, void(const QString &message, const qReal::Id &position));
	MOCK_METHOD3(sendBubblingMessage, void(const QString &message, int duration, QWidget *parent));
	MOCK_METHOD2(reportOperation, void(const QFuture<void> & operation, const QString &description));

	MOCK_METHOD0(clear, void());
	MOCK_METHOD0(clearErrors, void());
	MOCK_METHOD0(wereErrors, bool());
};

}
