#pragma once

#include <QtWidgets/QTreeView>
#include <QtWidgets/QAction>

#include "controller/controller.h"
#include "models/exploser.h"
#include "plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {
namespace gui {

class ModelExplorer : public QTreeView
{
	Q_OBJECT

public:
	explicit ModelExplorer(QWidget *parent = 0);

	void setController(Controller * const controller);
	void setAssistApi(details::ModelsAssistInterface * const model);
	void setExploser(models::Exploser const &exploser);

	/// Replaces current set of editor actions  with new one.
	/// Editor actions will be shown on context menu and their shortcuts will be triggered
	/// when model explorers are active.
	void changeEditorActionsSet(QList<QAction *> const &actions);

signals:
	/// Emitted when user wants to remove an element from the model.
	/// @todo: The implementation of the deletion process is currently placed to main window.
	/// Model explorers must be an independent components (maybe even pluginized).
	void elementRemoved();

protected:
	void commitData(QWidget *editor) override;
	void focusOutEvent(QFocusEvent *event) override;
	void focusInEvent(QFocusEvent *event) override;

private:
	void setActionsEnabled(bool enabled);

	Controller *mController; // Doesn`t take ownership
	details::ModelsAssistInterface *mModel; // Doesn`t take ownership
	models::Exploser const *mExploser; // Doesn`t take ownership
	QAction mDeleteAction;
	QAction mDeleteActionSeparator;
};

}
}
