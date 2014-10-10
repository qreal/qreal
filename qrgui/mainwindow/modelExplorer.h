#pragma once

#include <QtWidgets/QTreeView>

#include "controller/controller.h"
#include "pluginManager/exploser.h"
#include "toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {
namespace gui {

class ModelExplorer : public QTreeView
{
	Q_OBJECT

public:
	explicit ModelExplorer(QWidget *parent = 0);

	void setController(Controller * const controller);
	void setAssistApi(details::ModelsAssistInterface * const model);
	void setExploser(Exploser &exploser);
	void changeActionsSet(QList<QAction *> const &actions);

protected:
	virtual void commitData(QWidget *editor);
	void focusOutEvent(QFocusEvent* event);
	void focusInEvent(QFocusEvent * event);

private:
	void setActionsEnabled(bool enabled);

	Controller *mController; // Doesn`t take ownership
	details::ModelsAssistInterface *mModel; // Doesn`t take ownership
	Exploser *mExploser; // Doesn`t take ownership
};

}
}
