#pragma once

#include <QtWidgets/QTreeView>

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
	void changeActionsSet(QList<QAction *> const &actions);

protected:
	virtual void commitData(QWidget *editor);
	void focusOutEvent(QFocusEvent* event);
	void focusInEvent(QFocusEvent * event);

private:
	void setActionsEnabled(bool enabled);

	Controller *mController; // Doesn`t take ownership
	details::ModelsAssistInterface *mModel; // Doesn`t take ownership
	models::Exploser const *mExploser; // Doesn`t take ownership
};

}
}
