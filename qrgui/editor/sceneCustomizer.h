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

#include "qrgui/editor/editorDeclSpec.h"
#include "qrgui/editor/sceneCustomizationInterface.h"

namespace qReal {
namespace gui {
namespace editor {

/// Provides some information for the scene from plugins.
/// @todo: All entities in this class must be specified in the medamodel!
class QRGUI_EDITOR_EXPORT SceneCustomizer : public SceneCustomizationInterface
{
public:
	SceneCustomizer();

	QString addExplosionMenuName() const override;
	QString changeExplosionMenuName() const override;
	QString deleteExplosionMenuName() const override;
	QString goToConnectedMenuName() const override;
	bool showExpandConnectionAction() const override;
	QString expandExplosionActionText() const override;
	QString collapseExplosionActionText() const override;

	void setAddExplosionMenuName(const QString &text) override;
	void setChangeExplosionMenuName(const QString &text) override;
	void setDeleteExplosionMenuName(const QString &text) override;
	void setGoToConnectedMenuName(const QString &text) override;
	void setShowExpandConnectionAction(bool show) override;
	void setExpandExplosionActionText(const QString &text) override;
	void setCollapseExplosionActionText(const QString &text) override;

private:
	QString mAddExplosionMenuName;
	QString mChangeExplosionMenuName;
	QString mDeleteExplosionMenuName;
	QString mGoToConnectedMenuName;
	bool mShowExpandConnectionAction;
	QString mExpandExplosionActionText;
	QString mCollapseExplosionActionText;
};

}
}
}
