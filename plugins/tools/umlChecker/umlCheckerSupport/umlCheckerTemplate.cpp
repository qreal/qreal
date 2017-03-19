#include "umlCheckerTemplate.h"

#include <QtWidgets/QMessageBox>

using namespace qReal;

UmlCheckerTemplate::UmlCheckerTemplate(gui::MainWindowInterpretersInterface *mainWindowIFace
		, qrRepo::RepoControlInterface *repoControlIFace)
		: mMainWindowIFace(mainWindowIFace)
		, mRepoControlIFace(repoControlIFace)
{
}


UmlCheckerTemplate::~UmlCheckerTemplate()
{

}


void UmlCheckerTemplate::saveTemplate()
{
	const QString tempDirPath = "/home/julia/qreal/qreal/plugins/tools/umlChecker/test/";
	mMainWindowIFace->saveDiagramAsAPictureToFile(tempDirPath + "temp.png");
	mRepoControlIFace->saveTo(tempDirPath + "temp.qrs");
	QMessageBox::information(nullptr, tr("Information"), tr("Saved successfully"), tr("Ok"));
}
