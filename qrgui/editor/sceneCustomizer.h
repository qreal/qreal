#pragma once

#include "qrgui/editor/editorDeclSpec.h"
#include "qrgui/editor/sceneCustomizationInterface.h"

namespace qReal {

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

	void setAddExplosionMenuName(QString const &text) override;
	void setChangeExplosionMenuName(QString const &text) override;
	void setDeleteExplosionMenuName(QString const &text) override;
	void setGoToConnectedMenuName(QString const &text) override;
	void setShowExpandConnectionAction(bool show) override;
	void setExpandExplosionActionText(QString const &text) override;
	void setCollapseExplosionActionText(QString const &text) override;

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
