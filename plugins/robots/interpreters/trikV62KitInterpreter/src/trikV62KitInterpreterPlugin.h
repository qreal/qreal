/* Copyright 2015 CyberTech Labs Ltd., Yurii Litvinov
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

#include <trikKitInterpreterCommon/trikKitInterpreterPluginBase.h>

namespace trik {

class TrikV62KitInterpreterPlugin : public TrikKitInterpreterPluginBase
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "trik.TrikV62KitInterpreterPlugin")

public:
	TrikV62KitInterpreterPlugin();

	QString kitId() const override;
	QString friendlyKitName() const override;
	QList<qReal::ActionInfo> customActions() override;

private slots:
	void testSmile();

private:
	QAction mAction;

};

}
