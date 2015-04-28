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

#include <qrgui/plugins/toolPluginInterface/customizer.h>

class BpmnCustomizer : public QObject, public qReal::Customizer
{
	Q_OBJECT
public:
	virtual QString windowTitle() const;
	virtual QString productVersion() const;

	virtual bool showInterpeterButton() const;
	virtual bool showConnectionRelatedMenus() const;

	virtual QString addExplosionMenuName() const;
	virtual QString changeExplosionMenuName() const;
	virtual QString deleteExplosionMenuName() const;

	virtual QString goToConnectedMenuName() const;

	virtual bool showExpandConnectionAction() const;
	virtual QString expandExplosionActionText() const;
	virtual QString collapseExplosionActionText() const;
};

