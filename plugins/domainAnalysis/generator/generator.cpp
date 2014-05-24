#include "generator.h"
#include <QtCore/QProcess>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include "../../../qrkernel/settingsManager.h"
#include "../../../qrmc/metaCompiler.h"


#include "../../../qrkernel/roles.h"

#include "../../../qrutils/outFile.h"
#include "../../../qrutils/nameNormalizer.h"

#include "../../../qrkernel/settingsManager.h"
#include "../../../qrgui/pluginManager/editorManager.h"

using namespace qReal;
using namespace domainAnalysis;
using namespace utils;
using namespace qrRepo;


generator::generator()
		: mGenerateMetamodelAction(NULL)
		, mRepoControlApi(NULL)
{
}

generator::~generator()
{
}

void generator::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalRepoApi = &configurator.logicalModelApi().mutableLogicalRepoApi();
	mRepoControlApi = &configurator.repoControlInterface();
}

//The control's connection with method "onGenerateMethod"
QList<ActionInfo> generator::actions()
{
	mGenerateMetamodelAction.setText(tr("Generate metamodel"));
	ActionInfo generateMetaModelActionInfo(&mGenerateMetamodelAction, "generators", "tools");
	connect(&mGenerateMetamodelAction, SIGNAL(triggered()), this, SLOT(onGenerateMetamodel()));

	return QList<ActionInfo>() << generateMetaModelActionInfo;
}

void generator::onGenerateMetamodel()
{

	QFile f("D:\\test.txt");
	mRepo = new qrRepo::RepoApi("testRepo.qrs");

	//testId - узел редактора
	Id const editorTestId("MetaEditor", "MetaEditor", "MetamodelDiagram", QUuid::createUuid().toString());
	mRepo->addChild(Id::rootId(), editorTestId);
	mRepo->setProperty(editorTestId,"name", "Editor");
	mRepo->setProperty(editorTestId,"displayedName", "Editor");
	//mRepo->saveAll();
	//узел диаграммы
	Id const diagramNode("MetaEditor", "MetaEditor", "MetaEditorDiagramNode", QUuid::createUuid().toString());
	mRepo->addChild(editorTestId, diagramNode);
	mRepo->setProperty(diagramNode,"name", "Diagram");
	mRepo->setProperty(diagramNode,"displayedName", "Diagram");
	mRepo->setProperty(diagramNode,"nodeName", "Diagram");

	if (f.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&f);
		IdList const metamodels = mLogicalRepoApi->children(Id::rootId());
		foreach (Id const key, metamodels)
		{

			QString const objectType = key.element();

			if (objectType == "DetailedFeature" && mLogicalRepoApi->isLogicalElement(key))
			{
				stream << mLogicalRepoApi->name(key)<<"\r\n";
				Id const nodeId("MetaEditor", "MetaEditor", "MetaEntityNode", QUuid::createUuid().toString());
				mRepo->addChild(diagramNode,nodeId);
				QString const &name = mLogicalRepoApi->name(key);
				mRepo->setProperty(nodeId,"name", name);
				mRepo->setProperty(nodeId, "displayedName", name);

				//mRepo->setProperty(nodeId,"shape",mLogicalRepoApi->stringProperty(key, "shape"));

			}

		}

		Id const edgeId("MetaEditor", "MetaEditor", "MetaEntityEdge", QUuid::createUuid().toString());
		mRepo->addChild(diagramNode,edgeId);
		mRepo->setProperty(edgeId,"name","Стрелка следования");
		mRepo->setProperty(edgeId, "displayedName", "Стрелка следования");
		mRepo->setProperty(edgeId,"shape", "broken");
		mRepo->setProperty(edgeId,"lineType", "solidLine");


		mRepo->saveTo("D:/testEditor.qrs");


		f.close();
		if (stream.status() != QTextStream::Ok)
		{
			qDebug() << "Ошибка записи файла";
		}

		//qrRepo::RepoApi *repoApi = new qrRepo::RepoApi(workingCopy);

	}


}
