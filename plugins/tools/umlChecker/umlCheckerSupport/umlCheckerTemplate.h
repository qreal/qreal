#pragma once
#include <QtWidgets/QMenu>
#include <QInputDialog>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>

#include <qrrepo/repoApi.h>

namespace qReal {

class UmlCheckerTemplate : public QObject
{
	Q_OBJECT
public:
	UmlCheckerTemplate(gui::MainWindowInterpretersInterface *mainWindowIFace
			, qrRepo::RepoControlInterface *repoControlIFace);
	~UmlCheckerTemplate();

	void saveEdgeTemplate();
	void saveTemplate();


private:
	gui::MainWindowInterpretersInterface *mMainWindowIFace;
	qrRepo::RepoControlInterface *mRepoControlIFace;

};

}
