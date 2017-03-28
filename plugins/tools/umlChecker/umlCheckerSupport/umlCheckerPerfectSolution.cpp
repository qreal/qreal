#include "umlCheckerPerfectSolution.h"

#include <QtWidgets/QMessageBox>
#include <QFileDialog>

using namespace qReal;

UmlCheckerPerfectSolution::UmlCheckerPerfectSolution(gui::MainWindowInterpretersInterface *mainWindowIFace
		, qrRepo::RepoControlInterface *repoControlIFace)
		: mMainWindowIFace(mainWindowIFace)
		, mRepoControlIFace(repoControlIFace)
{
	mLocationDirPath = "/home/julia/qreal/qreal/plugins/tools/umlChecker/perfect/";
	mPerfectRepoApi = new qrRepo::RepoApi("/home/julia/qreal/qreal/plugins/umlChecker/perfect", true);
}

UmlCheckerPerfectSolution::~UmlCheckerPerfectSolution()
{

}


void UmlCheckerPerfectSolution::saveTempSolution()
{
	mLocationDirPath = QFileDialog::getExistingDirectory(nullptr, tr("Specify directory:")) + "/";

	const QString fileName = mLocationDirPath + "temp" + ".qrs";
	mRepoControlIFace->saveTo(fileName);

	mPerfectRepoApi->open(fileName);
}


void UmlCheckerPerfectSolution::addElementsToBlock(const QString &blockName)
{
	IdList activeElements = mMainWindowIFace->selectedElementsOnActiveDiagram();
	for (Id &id : activeElements) {
		mPerfectRepoApi->setProperty(id, "blockName", QVariant(blockName));
	}


	saveAll();
}

void UmlCheckerPerfectSolution::saveOptionsForEdge(const QPair<QString, QStringList> &elements)
{
	QString fileName = mLocationDirPath + "edges/" + elements.first + ".txt";
	QFile file(fileName);
	if (file.open(QIODevice::ReadWrite)) {
		IdList activeElements = mMainWindowIFace->selectedElementsOnActiveDiagram();
		QTextStream stream(&file);

		for (Id &id : activeElements) {
			const Id from = mPerfectRepoApi->from(id);
			const Id to = mPerfectRepoApi->to(id);
			const QString blockFrom = mPerfectRepoApi->property(from, "blockName").toString();
			const QString blockTo = mPerfectRepoApi->property(to, "blockName").toString();
			stream << blockFrom << endl;
			stream << blockTo << endl;
			for (QString element : elements.second) {
				element.chop(4);
				stream << element << endl;
			}
		}

	}
}

void UmlCheckerPerfectSolution::saveOptionsForBlock(const QPair<QString, QStringList> &elements)
{
	QString fileName = mLocationDirPath + elements.first + ".txt";
	QFile file(fileName);
	if (file.open(QIODevice::ReadWrite)) {
		QTextStream stream(&file);
		for (QString element : elements.second) {
			element.chop(4);
			const QString fileNameOfBlock = element + ".qrs";
			stream << fileNameOfBlock << endl;
		}
	}

	file.close();
}


void UmlCheckerPerfectSolution::saveAll()
{
	mPerfectRepoApi->save(mPerfectRepoApi->graphicalElements());
	QMessageBox::information(nullptr, tr("Information"), tr("Saved successfully"), tr("Ok"));
}
