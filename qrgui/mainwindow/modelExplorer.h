#pragma once

#include <QtWidgets/QTreeView>

#include "controller/controller.h"
#include "models/details/exploser.h"
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
	void setExploser(models::Exploser * const exploser);

protected:
	virtual void commitData(QWidget *editor);

private:
	Controller *mController; // Doesn`t take ownership
	details::ModelsAssistInterface *mModel; // Doesn`t take ownership
	models::Exploser *mExploser; // Doesn`t take ownership
};

}
}
