/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#include <qrgui/plugins/toolPluginInterface/customizer.h>

namespace interpreterCore {

/// Configures QReal's main GUI module disabling some unneeded stuff and customizing such
/// aspects as window title, window icon and so on.
class Customizer : public qReal::Customizer
{
public:
	QString windowTitle() const override;
	QIcon applicationIcon() const override;
	QImage applicationLogo() const override;
	QString productVersion() const override;
	QString aboutText() const override;
	QString examplesDirectory() const override;

	QList<QPair<QString, qReal::ActionVisibility>> actionsVisibility() const override;

	bool showInterpeterButton() const override;

	QString userPaletteTitle() const override;
	QString userPaletteDescription() const override;

private:
};

}
