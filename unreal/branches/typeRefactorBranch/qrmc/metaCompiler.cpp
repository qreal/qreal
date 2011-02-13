#include "metaCompiler.h"
#include "editor.h"
#include "utils/nameNormalizer.h"
#include "diagram.h"

#include "classes/type.h"
#include "classes/edgeType.h"
#include "classes/nodeType.h"
#include "classes/enumType.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

using namespace qReal;
using namespace qrmc;

MetaCompiler::MetaCompiler(QString const &qrmcDir, QString const &workingCopyDir) : mApi(workingCopyDir)
{
	mLocalDir = qrmcDir;
	mApi.setName(ROOT_ID, ROOT_ID.toString());
	loadTemplateFromFile(pluginHeaderTemplate, mPluginHeaderTemplate);
	loadTemplateFromFile(pluginSourceTemplate, mPluginSourceTemplate);
	loadTemplateFromFile(nodeClassTemplate, mNodeTemplate);
	loadTemplateFromFile(edgeClassTemplate, mEdgeTemplate);
	loadTemplateFromFile(elementsHeaderTemplate, mElementsHeaderTemplate);
	loadTemplateFromFile(resourceTemplate, mResourceTemplate);
	loadTemplateFromFile(projectTemplate, mProjectTemplate);
	loadTemplateFromFile(pluginsProjectTemplate, mPluginsProjectTemplate);
	loadTemplateUtils();
}

MetaCompiler::~MetaCompiler()
{
	foreach(Editor *editor, mEditors.values())
		if (editor)
			delete editor;
}

bool MetaCompiler::compile(QString const &targetMetamodel)
{
	mTargetMetamodel = targetMetamodel;
        IdList rootItems = mApi.children(ROOT_ID);
	qDebug() << "root diagrams:" << rootItems.size();
	if (rootItems.isEmpty())
		qDebug() << "couldn't load any root diagrams";
        foreach(qReal::Id editorId, rootItems) {
            NewType editorType = mApi.type(editorId);
                if (editorType.element() == metamodelDiagram) {
                        if (!mTargetMetamodel.isEmpty() && mApi.name(editorId) != mTargetMetamodel )
				continue;
			mPluginName = NameNormalizer::normalize(mApi.property(editorId, nameOfTheDirectory)
											.toString().section("/", -1));
                        if (!loadMetaModel(editorId))
				return false;
		}
	}
	generateCode();
	return true;
}

bool MetaCompiler::changeDir(const QString &path)
{
	if (!mDirectory.exists(path)) {
		qDebug() << "cannot find directory " << path;
		return false;
	}
	mDirectory.cd(path);
	return true;
}

bool MetaCompiler::loadTemplateFromFile(QString const &templateFileName, QString &loadedTemplate)
{
	if (!changeDir(mLocalDir + "/" + templatesDir))
		return false;

	QString fileName = mDirectory.absoluteFilePath(templateFileName);
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&file);
	loadedTemplate = in.readAll();

	file.close();
	mDirectory.cdUp();

	return true;
}

bool MetaCompiler::loadTemplateUtils()
{
	if (!changeDir(mLocalDir + "/" + templatesDir))
		return false;

	QString fileName = mDirectory.absoluteFilePath(utilsTemplate);
	QFile utilsFile(fileName);
	if (!utilsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&utilsFile);
	QString line = in.readLine();
	while (!line.isNull()) {
		QString name = line.section("=", 0, 0);
		QString definition = line.section("=", 1);
		mTemplateUtils[name] = definition;
		line = in.readLine();
	}

	utilsFile.close();
	mDirectory.cdUp();

	return true;
}


Editor* MetaCompiler::loadMetaModel(Id const &metamodelId)
{
        qDebug() << "Loading metamodel started: " << mApi.name(metamodelId);
        QString metamodelName = mApi.name(metamodelId);

	if (mEditors.contains(metamodelName)) {
		Editor *editor = mEditors[metamodelName];
		if (editor->isLoaded()) {
			qDebug() << "Metamodel already loaded";
			return editor;
		} else {
			qDebug() << "ERROR: cycle detected";
			return NULL;
		}
	} else {
		Editor *editor = new Editor(this, &mApi, metamodelId);
		if (!editor->load()) {
			qDebug() << "ERROR: Failed to load file";
			delete editor;
			return NULL;
		}
		mEditors[metamodelName] = editor;
		return editor;
	}
}

Diagram *MetaCompiler::getDiagram(QString const &diagramName)
{
	foreach (Editor *editor, mEditors) {
		Diagram *diagram = editor->findDiagram(diagramName);
		if (diagram)
			return diagram;
	}
	return NULL;
}

void MetaCompiler::generateCode()
{
	qDebug() << "loaded metamodels: " << mEditors.keys();
	qDebug() << "===";

	QString pluginNames;
	foreach (Editor *editor, mEditors) {
                if (!mTargetMetamodel.isEmpty() && mApi.name(editor->type()) != mTargetMetamodel )
			continue;
		pluginNames += nodeIndent + editor->name() + "\\" + endline;
		editor->generate(mPluginHeaderTemplate, mPluginSourceTemplate,
					mNodeTemplate, mEdgeTemplate, mElementsHeaderTemplate,
					mResourceTemplate, mProjectTemplate, mTemplateUtils);
	}

	QDir dir;

	if (!dir.exists(generatedDir))
		dir.mkdir(generatedDir);
	dir.cd(generatedDir);

	QString const fileName = dir.absoluteFilePath(pluginsProjectFileName);
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return;
	}

	QTextStream out(&file);
	QString projectTemplate = mPluginsProjectTemplate;
	out << projectTemplate.replace(subdirsTag, pluginNames);
	file.close();

	return;
}

QString MetaCompiler::getTemplateUtils(const QString &tmpl) const
{
	return mTemplateUtils[tmpl];
}
