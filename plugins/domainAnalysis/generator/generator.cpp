#include "generator.h"
#include <QtCore/QProcess>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <qrkernel/settingsManager.h>
#include <qrmc/metaCompiler.h>
#include <qrkernel/roles.h>
#include <qrutils/outFile.h>
#include <qrutils/nameNormalizer.h>
#include <qrkernel/settingsManager.h>
#include <qrgui/pluginManager/editorManager.h>

using namespace qReal;
using namespace domainAnalysis;
using namespace utils;
using namespace qrRepo;

Generator::Generator()
		: mGenerateMetamodelAction(nullptr)
		, mRepoControlApi(nullptr)
{
}

Generator::~Generator()
{
}

void Generator::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalRepoApi = &configurator.logicalModelApi().mutableLogicalRepoApi();
	mRepoControlApi = &configurator.repoControlInterface();
}

//The control's connection with method "onGenerateMethod"
QList<ActionInfo> Generator::actions()
{
	mGenerateMetamodelAction.setText(tr("Generate metamodel"));
	ActionInfo generateMetaModelActionInfo(&mGenerateMetamodelAction, "generators", "tools");
	QObject::connect(&mGenerateMetamodelAction, &QAction::triggered, this, &Generator::onGenerateMetamodel);
	return QList<ActionInfo>() << generateMetaModelActionInfo;
}

void Generator::onGenerateMetamodel()
{
	QFile testFile("test.txt");
	mRepo = new qrRepo::RepoApi("testRepo.qrs");
	//testId - editorNode
	Id const editorTestId("MetaEditor", "MetaEditor", "MetamodelDiagram", QUuid::createUuid().toString());
	mRepo->addChild(Id::rootId(), editorTestId);
	mRepo->setProperty(editorTestId,"name", "Editor");
	mRepo->setProperty(editorTestId,"displayedName", "Editor");
	//DiagramNode
	Id const diagramNode("MetaEditor", "MetaEditor", "MetaEditorDiagramNode", QUuid::createUuid().toString());
	mRepo->addChild(editorTestId, diagramNode);
	mRepo->setProperty(diagramNode,"name", "Diagram");
	mRepo->setProperty(diagramNode,"displayedName", "Diagram");
	mRepo->setProperty(diagramNode,"nodeName", "Diagram");
	if (testFile.open(QIODevice::WriteOnly)) {
		QTextStream stream(&testFile);
		IdList const metamodels = mLogicalRepoApi->children(Id::rootId());
		for (Id const key: metamodels) {
			QString const objectType = key.element();
			if (objectType == "DetailedFeature" && mLogicalRepoApi->isLogicalElement(key)) {
				stream << mLogicalRepoApi->name(key)<<"\r\n";
				Id const nodeId("MetaEditor", "MetaEditor", "MetaEntityNode", QUuid::createUuid().toString());
				mRepo->addChild(diagramNode,nodeId);
				QString const &name = mLogicalRepoApi->name(key);
				mRepo->setProperty(nodeId,"name", name);
				mRepo->setProperty(nodeId, "displayedName", name);
			}
		}
		Id const edgeId("MetaEditor", "MetaEditor", "MetaEntityEdge", QUuid::createUuid().toString());
		mRepo->addChild(diagramNode,edgeId);
		mRepo->setProperty(edgeId,"name","Стрелка следования");
		mRepo->setProperty(edgeId, "displayedName", "Стрелка следования");
		mRepo->setProperty(edgeId,"shape", "broken");
		mRepo->setProperty(edgeId,"lineType", "solidLine");
		mRepo->saveTo("testEditor.qrs");
		testFile.close();
		if (stream.status() != QTextStream::Ok) {
			qDebug() << "File writing erorr";
		}
	}
}
