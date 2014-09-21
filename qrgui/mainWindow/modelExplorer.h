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
	void setExploser(Exploser &exploser);

protected:
	virtual void commitData(QWidget *editor);

private:
	Controller *mController; // Doesn`t take ownership
	details::ModelsAssistInterface *mModel; // Doesn`t take ownership
	Exploser *mExploser; // Doesn`t take ownership
};

}
}
