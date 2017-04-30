#include "umlCheckerTemplate.h"

#include <QtWidgets/QMessageBox>
#include <QFileDialog>

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
	const QString templateName = QInputDialog::getText(nullptr, tr("enter template name"), tr("enter template name"));
	const QString tempDirPath = QFileDialog::getExistingDirectory(nullptr, tr("Specify directory:")) + "/";
	mMainWindowIFace->saveDiagramAsAPictureToFile(tempDirPath + templateName + ".png");
	mRepoControlIFace->saveTo(tempDirPath + templateName + ".qrs");
	QMessageBox::information(nullptr, tr("Information"), tr("Saved successfully"), tr("Ok"));
}

void UmlCheckerTemplate::saveEdgeTemplate()
{
	const QString templateName = QInputDialog::getText(nullptr, tr("enter template name"), tr("enter template name"));
	const QString tempDirPath = QFileDialog::getExistingDirectory(nullptr, tr("Specify directory:")) + "/";
	mMainWindowIFace->saveDiagramAsAPictureToFile(tempDirPath + templateName + ".png");
	QMessageBox::information(nullptr, tr("Information"), tr("Saved successfully"), tr("Ok"));
}
