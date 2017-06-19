/* Copyright 2016-2017 CyberTech Labs Ltd.
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

#include <QtCore/QSharedPointer>
#include <QtCore/QScopedPointer>
#include <QtCore/QHash>

#include <utils/abstractTimer.h>
#include <trikControl/keysInterface.h>

namespace kitBase {
namespace robotModel {
class CommonRobotModel;
namespace robotParts {
class Button;
}
}
}


namespace trik {

class TrikKeysInterfaceStub : public trikControl::KeysInterface
{

public:
	TrikKeysInterfaceStub(const QSharedPointer<kitBase::robotModel::CommonRobotModel> &model);

	void init();
	void start();
	void stop();
	virtual Status status() const override { return Status::ready; }

public slots:
	virtual void reset() override;
	virtual bool wasPressed(int code) override;
	virtual bool isPressed(int code) override;
	virtual int buttonCode(bool wait) override;

private slots:
	void handleNewData(int value);

private:
	bool registerButton(int code);

	QHash<int, QString> mKeycodeMap;
	QHash<int, bool> mWasPressed;
	QHash<int, kitBase::robotModel::robotParts::Button *> mButtons;
	QSharedPointer<kitBase::robotModel::CommonRobotModel> mRobotModel;
	QScopedPointer<utils::AbstractTimer> mButtonWatchingTimer;
};

}
