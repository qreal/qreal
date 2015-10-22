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

//#include <kitBase/kitPluginInterface.h>
//#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <trikKitInterpreterCommon/trikKitInterpreterPluginBase.h>

namespace testbutton {

/// Test
class TestButtonPlugin : public trik::TrikKitInterpreterPluginBase
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "testbutton.TestButtonPlugin")

public:
	TestButtonPlugin();
	~TestButtonPlugin() override;

	QList<qReal::ActionInfo> customActions() override;

	QString kitId() const override {return QString("4242");}

	QString friendlyKitName() const override {return QString("abc");}

	int priority() const override {return 10;}

//	void init(qReal::PluginConfigurator const &configurator) override;

private slots:
	/// Deletion of all items from repository.
	void exterminate();

private:
	qrRepo::RepoControlInterface *mRepo;  // Doesn't have ownership
	QAction mAction;

	/// To syncronize logical and graphical models with repository.
	/// Doesn`t have ownership.
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;

};

}
