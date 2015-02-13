#pragma once

#include <QtCore/QString>

namespace qReal {

/// Provides some methods for QReal scene customization
/// @todo Get rid of it cause all entities here must be specified in metamodel.
class SceneCustomizationInterface
{
public:
	virtual ~SceneCustomizationInterface() {}

	/// Returns an explosion addition menu text that will be displayed in element`s context menu.
	virtual QString addExplosionMenuName() const = 0;

	/// Returns an explosion name modification menu text that will be displayed in element`s context menu.
	virtual QString changeExplosionMenuName() const = 0;

	/// Returns an explosion deletion menu text that will be displayed in element`s context menu.
	virtual QString deleteExplosionMenuName() const = 0;

	/// Returns an explosion transition action text that will be displayed in element`s context menu.
	virtual QString goToConnectedMenuName() const = 0;

	/// Returns true if expasion actions must be displayed in element`s context menu.
	virtual bool showExpandConnectionAction() const = 0;

	/// Returns an explosion expansion action text that will be displayed in element`s context menu.
	virtual QString expandExplosionActionText() const = 0;

	/// Returns an explosion collapse action text that will be displayed in element`s context menu.
	virtual QString collapseExplosionActionText() const = 0;

	/// Customizes an explosion addition menu text that will be displayed in element`s context menu.
	virtual void setAddExplosionMenuName(const QString &text) = 0;

	/// Customizes an explosion name modification menu text that will be displayed in element`s context menu.
	virtual void setChangeExplosionMenuName(const QString &text) = 0;

	/// Customizes an explosion deletion menu text that will be displayed in element`s context menu.
	virtual void setDeleteExplosionMenuName(const QString &text) = 0;

	/// Customizes an explosion transition action text that will be displayed in element`s context menu.
	virtual void setGoToConnectedMenuName(const QString &text) = 0;

	/// Shows or hides expansion actions in the element`s context menu.
	virtual void setShowExpandConnectionAction(bool show) = 0;

	/// Customizes an explosion expansion action text that will be displayed in element`s context menu.
	virtual void setExpandExplosionActionText(const QString &text) = 0;

	/// Customizes an explosion collapse action text that will be displayed in element`s context menu.
	virtual void setCollapseExplosionActionText(const QString &text) = 0;

};

}
