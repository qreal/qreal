#pragma once

#include <QtWidgets/QMenu>
#include <QInputDialog>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>

#include <qrrepo/repoApi.h>

namespace qReal {
class UmlCheckerPerfectSolution : public QObject
{
	Q_OBJECT
public:
	UmlCheckerPerfectSolution();
	~UmlCheckerPerfectSolution();

	UmlCheckerPerfectSolution(gui::MainWindowInterpretersInterface *mainWindowIFace
			, qrRepo::RepoControlInterface *repoControlIFace);

	void saveTempSolution();
	void addElementsToBlock(const QString &blockName);
	void saveAll();
	void saveOptionsForBlock(const QPair<QString, QStringList> &elements);

private:
	gui::MainWindowInterpretersInterface *mMainWindowIFace;
	qrRepo::RepoControlInterface *mRepoControlIFace;
	qrRepo::RepoApi *mPerfectRepoApi;

	//TODO
	QString mLocationDirPath;
};

}
