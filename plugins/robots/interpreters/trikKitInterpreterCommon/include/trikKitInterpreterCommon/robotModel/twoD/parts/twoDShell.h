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

#include <trikKit/robotModel/parts/trikShell.h>

#include "trikKitInterpreterCommon/declSpec.h"

namespace qReal {
class ErrorReporterInterface;
}

namespace twoDModel {
namespace engine {
class TwoDModelEngineInterface;
}
}

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

/// Emulates shell in 2D model. runCommand() methods do nothing, other operations are supported.
class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT Shell : public robotModel::parts::TrikShell
{
	Q_OBJECT
	Q_PROPERTY(QString lastPhrase READ lastPhrase WRITE say NOTIFY phraseTold)

public:
	Shell(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void runCommand(const QString &command) override;
	void runCode(const QString &code) override;
	void say(const QString &text) override;
	void writeToFile(const QString &filePath, const QString &text) override;
	void removeFile(const QString &filePath) override;
	void print(const QString &text) override;
	void initVideoStreaming() override;

	/// Sets the error reporter for writing bubbling messages in say().
	void setErrorReporter(qReal::ErrorReporterInterface &errorReporter);

	/// Returns the value passed to say() method last time or empty string if it was not called at the moment.
	QString lastPhrase() const;

	/// Resets shell instance preparing it to new interpretation time.
	void reset();

signals:
	/// Emitted when robot starts speaking some phrase; the told text is passed as argument.
	void phraseTold(const QString &text);

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
	qReal::ErrorReporterInterface *mErrorReporter;
	QString mLastPhrase;
};

}
}
}
}
