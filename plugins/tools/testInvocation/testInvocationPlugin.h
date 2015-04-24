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

#include <QtCore/QObject>

#include "../../qrgui/toolPluginInterface/toolPluginInterface.h"

namespace testInvocation
{

/// This plugin shows how to use invocation framework
class TestInvocationPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	virtual QList<qReal::ActionInfo> actions();
	virtual void init(qReal::PluginConfigurator const &configurator);

	void doOperation(invocation::Progress *progress, int sleepInterval, int count);
	void doOperation(int sleepInterval, int count);

private slots:
	void asyncWithoutProgress();
	void asyncWithProgress();
	void syncWithoutProgress();
	void syncWithProgress();

	void onOpertionFinished(invocation::InvocationState result);
private:
	qReal::gui::MainWindowInterpretersInterface *mMainWindow;

};

}
