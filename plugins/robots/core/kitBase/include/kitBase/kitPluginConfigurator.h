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

#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrtext/languageToolboxInterface.h>
#include <kitBase/eventsForKitPluginInterface.h>
#include <kitBase/interpreterControlInterface.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>

namespace kitBase {

/// Container class for convenient passing different robots plugin parts to kit plugins
class KitPluginConfigurator
{
public:
	KitPluginConfigurator(const qReal::PluginConfigurator &qRealConfigurator
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const kitBase::EventsForKitPluginInterface &eventsForKitPlugin
			, kitBase::InterpreterControlInterface &interpreterControl)
		: mQRealConfigurator(qRealConfigurator)
		, mRobotModelManager(robotModelManager)
		, mTextLanguage(textLanguage)
		, mEventsForKitPlugin(eventsForKitPlugin)
		, mInterpreterControl(interpreterControl)
	{
	}

	/// Returns a reference to QReal engine`s plugin configurator used for tool plugin configuration.
	const qReal::PluginConfigurator &qRealConfigurator() const
	{
		return mQRealConfigurator;
	}

	/// Returns a reference to an object that keeps tracking of the current selected robot model.
	const kitBase::robotModel::RobotModelManagerInterface &robotModelManager() const
	{
		return mRobotModelManager;
	}

	/// Returns a reference to text language infrustructure (parser, analizer and interpreter on some language).
	qrtext::LanguageToolboxInterface &textLanguage() const
	{
		return mTextLanguage;
	}

	/// Returns a reference to an object with events raised in interpreter that plugin can use to perform custom
	/// actions, for example, starting and stopping of interpretation.
	const kitBase::EventsForKitPluginInterface &eventsForKitPlugin() const
	{
		return mEventsForKitPlugin;
	}

	/// Returns a reference to an object that can be used for communication with the interpreter, for example starting
	/// and stopping it.
	kitBase::InterpreterControlInterface &interpreterControl() const
	{
		return mInterpreterControl;
	}

private:
	const qReal::PluginConfigurator &mQRealConfigurator;
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
	qrtext::LanguageToolboxInterface &mTextLanguage;
	const kitBase::EventsForKitPluginInterface &mEventsForKitPlugin;
	kitBase::InterpreterControlInterface &mInterpreterControl;
};

}
