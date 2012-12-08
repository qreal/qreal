#pragma once

#include "../../qrgui/toolPluginInterface/customToolInterface.h"
#include "../../qrrepo/repoControlInterface.h"

namespace exterminatus
{

class ExterminatusTool : public QObject, public qReal::CustomToolInterface
{
	Q_OBJECT

public:
	ExterminatusTool();
	virtual ~ExterminatusTool();
	virtual QList<ActionInfo> actions();
	virtual void init(qrRepo::RepoControlInterface *repo);

private slots:
	void exterminate();

private:
	qrRepo::RepoControlInterface *mRepo;  // Doesn't have ownership
	QAction mAction;
};

}
