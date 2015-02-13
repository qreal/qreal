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
