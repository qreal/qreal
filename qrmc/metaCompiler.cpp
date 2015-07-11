/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

MetaCompiler::MetaCompiler(const QString &qrmcDir, qrRepo::LogicalRepoApi *mLogicalRepoApi) : mApi(mLogicalRepoApi)
{
	mLocalDir = qrmcDir;
	mApi->setName(Id::rootId(), Id::rootId().toString());
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
	foreach(Editor *editor, mEditors.values()) {
		if (editor) {
			delete editor;
		}
	}
}

bool MetaCompiler::compile(const QString &targetMetamodel)
{
	mTargetMetamodel = targetMetamodel;
	IdList rootItems = mApi->children(Id::rootId());
	qDebug() << "root diagrams:" << rootItems.size();
	if (rootItems.isEmpty())
		qDebug() << "couldn't load any root diagrams";
	foreach(qReal::Id editorId, rootItems) {
		if (!mApi->isLogicalElement(editorId))
			continue;

		if (editorId.element() == metamodelDiagram) {
			if (!mTargetMetamodel.isEmpty() && mApi->name(editorId) != mTargetMetamodel)
				continue;
			mPluginName = NameNormalizer::normalize(mApi->property(editorId, nameOfTheDirectory)
											.toString().section("/", -1));
			if (!loadMetaModel(editorId)) {
				return false;
			}
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

bool MetaCompiler::loadTemplateFromFile(const QString &templateFileName, QString &loadedTemplate)
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


Editor* MetaCompiler::loadMetaModel(const Id &metamodelId)
{
	qDebug() << "Loading metamodel started: " << mApi->name(metamodelId);
	QString metamodelName = mApi->name(metamodelId);

	if (mEditors.contains(metamodelName)) {
		Editor *editor = mEditors[metamodelName];
		if (editor->isLoaded()) {
			qDebug() << "Metamodel already loaded";
			return editor;
		} else {
			qDebug() << "ERROR: cycle detected";
			return nullptr;
		}
	} else {
		Editor *editor = new Editor(this, mApi, metamodelId);
		if (!editor->load()) {
			qDebug() << "ERROR: Failed to load file";
			delete editor;
			return nullptr;
		}
		mEditors[metamodelName] = editor;
		return editor;
	}
}

Diagram *MetaCompiler::getDiagram(const QString &diagramName)
{
	foreach (Editor *editor, mEditors) {
		Diagram *diagram = editor->findDiagram(diagramName);
		if (diagram)
			return diagram;
	}
	return nullptr;
}

void MetaCompiler::generateCode()
{
	qDebug() << "loaded metamodels: " << mEditors.keys();
	qDebug() << "===";

	QString pluginNames;
	foreach (Editor *editor, mEditors) {
		if (!mTargetMetamodel.isEmpty() && mApi->name(editor->id()) != mTargetMetamodel )
			continue;
		pluginNames += nodeIndent + editor->name() + "\\" + endline;
		editor->generate(mPluginHeaderTemplate, mPluginSourceTemplate,
					mNodeTemplate, mEdgeTemplate, mElementsHeaderTemplate,
					mResourceTemplate, mProjectTemplate, mTemplateUtils);
	}

	QDir dir;

	if (!dir.exists(generatedDir)) {
		dir.mkdir(generatedDir);
	}
	dir.cd(generatedDir);

	const QString fileName = dir.absoluteFilePath(pluginsProjectFileName);
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return;
	}

	QTextStream out(&file);
	out.setCodec("UTF-8");
	QString projectTemplate = mPluginsProjectTemplate;
	out << projectTemplate.replace(subdirsTag, pluginNames);
	file.close();

	return;
}

QString MetaCompiler::getTemplateUtils(const QString &tmpl) const
{
	return mTemplateUtils[tmpl];
}
