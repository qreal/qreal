#include "umlCheckerPerfectSolution.h"

#include <QtWidgets/QMessageBox>

using namespace qReal;

UmlCheckerPerfectSolution::UmlCheckerPerfectSolution(gui::MainWindowInterpretersInterface *mainWindowIFace
		, qrRepo::RepoControlInterface *repoControlIFace)
		: mMainWindowIFace(mainWindowIFace)
		, mRepoControlIFace(repoControlIFace)
{
	mTempDirPath = "/home/julia/qreal/qreal/plugins/tools/umlChecker/perfect/";
	mPerfectRepoApi = new qrRepo::RepoApi("/home/julia/qreal/qreal/plugins/umlChecker/perfect", true);
}

UmlCheckerPerfectSolution::~UmlCheckerPerfectSolution()
{

}


void UmlCheckerPerfectSolution::saveTempSolution()
{
	const QString fileName = mTempDirPath + "temp.qrs";
	mRepoControlIFace->saveTo(fileName);

	mPerfectRepoApi->open(fileName);
}


void UmlCheckerPerfectSolution::addElementsToBlock(const QString &blockName)
{
	IdList activeElements = mMainWindowIFace->selectedElementsOnActiveDiagram();
	for (Id &id : activeElements) {
		mPerfectRepoApi->setProperty(id, blockName, QVariant(""));
	}


	saveAll();
}

void UmlCheckerPerfectSolution::saveOptionsForBlock(const QPair<QString, QStringList> &elements)
{
	QString filename= mTempDirPath + elements.first + ".txt";
	QFile file(filename);
	if (file.open(QIODevice::ReadWrite))
	{
		QTextStream stream(&file);
		stream << elements.first << endl;

		for (QString element : elements.second)
		{
			element.chop(4);
			const QString fileName = element + ".qrs";
			stream << fileName << endl;
		}
	}

	file.close();
}


void UmlCheckerPerfectSolution::saveAll()
{
	mPerfectRepoApi->save(mPerfectRepoApi->graphicalElements());
	QMessageBox::information(nullptr, tr("Information"), tr("Saved successfully"), tr("Ok"));
}
