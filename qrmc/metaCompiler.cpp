/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include "editor.h"
#include "utils/nameNormalizer.h"
#include "diagram.h"

#include "classes/type.h"
#include "classes/edgeType.h"
#include "classes/nodeType.h"
#include "classes/enumType.h"

using namespace qReal;
using namespace qrmc;

MetaCompiler::MetaCompiler(const qrRepo::LogicalRepoApi &logicalRepoApi, const QString &targetDirectory)
	: mApi(logicalRepoApi)
	, mTargetDirectory(targetDirectory)
{
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
	qDeleteAll(mEditors);
}

bool MetaCompiler::compile(const QString &targetMetamodel)
{
	const IdList rootItems = mApi.children(Id::rootId());
	qDebug() << "root diagrams:" << rootItems.size();
	if (rootItems.isEmpty()) {
		qDebug() << "couldn't load any root diagrams";
		return false;
	}

	for (const qReal::Id &editorId : rootItems) {
		if (!mApi.isLogicalElement(editorId)) {
			continue;
		}

		if (editorId.element() == metamodelDiagram) {
			if (!targetMetamodel.isEmpty() && mApi.name(editorId) != targetMetamodel) {
				continue;
			}

			if (!loadMetaModel(editorId)) {
				return false;
			}
		}
	}

	generateCode(targetMetamodel);
	return true;
}

bool MetaCompiler::loadTemplateFromFile(const QString &templateFileName, QString &loadedTemplate)
{
	const QFileInfo fileInfo(templatesDir + "/" + templateFileName);
	QFile file(fileInfo.canonicalFilePath());
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Cannot load file \"" << fileInfo.canonicalFilePath() << "\".";
		return false;
	}

	QTextStream in(&file);
	loadedTemplate = in.readAll();

	file.close();
	return true;
}

bool MetaCompiler::loadTemplateUtils()
{
	const QFileInfo fileInfo(templatesDir + "/" + utilsTemplate);
	QFile utilsFile(fileInfo.canonicalFilePath());
	if (!utilsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileInfo.canonicalFilePath() << "\"";
		return false;
	}

	QTextStream in(&utilsFile);
	QString line = in.readLine();
	while (!line.isNull()) {
		const QString name = line.section("=", 0, 0);
		const QString definition = line.section("=", 1);
		mTemplateUtils[name] = definition;
		line = in.readLine();
	}

	utilsFile.close();
	return true;
}

Editor* MetaCompiler::loadMetaModel(const Id &metamodelId)
{
	qDebug() << "Loading metamodel started: " << mApi.name(metamodelId);
	const QString metamodelName = mApi.name(metamodelId);

	if (mEditors.contains(metamodelName)) {
		Editor * const editor = mEditors[metamodelName];
		if (editor->isLoaded()) {
			qDebug() << "Metamodel already loaded";
			return editor;
		} else {
			qDebug() << "ERROR: cycle detected";
			return nullptr;
		}
	} else {
		Editor * const editor = new Editor(*this, mApi, metamodelId, mTargetDirectory);
		if (!editor->load()) {
			qDebug() << "ERROR: Failed to load file";
			delete editor;
			return nullptr;
		}

		mEditors[metamodelName] = editor;
		return editor;
	}
}

Diagram *MetaCompiler::getDiagram(const QString &diagramName) const
{
	for (const Editor * const editor : mEditors) {
		Diagram * const diagram = editor->findDiagram(diagramName);
		if (diagram) {
			return diagram;
		}
	}

	return nullptr;
}

void MetaCompiler::generateCode(const QString &targetMetamodel)
{
	qDebug() << "loaded metamodels: " << mEditors.keys();
	qDebug() << "===";

	QString pluginNames;
	for (Editor * const editor : mEditors) {
		if (!targetMetamodel.isEmpty() && mApi.name(editor->id()) != targetMetamodel) {
			continue;
		}

		pluginNames += nodeIndent + editor->name() + "\\" + endline;
		editor->generate(mPluginHeaderTemplate, mPluginSourceTemplate,
					mNodeTemplate, mEdgeTemplate, mElementsHeaderTemplate,
					mResourceTemplate, mProjectTemplate, mTemplateUtils);
	}

	QDir dir;

	if (!dir.exists(mTargetDirectory)) {
		dir.mkdir(mTargetDirectory);
	}

	dir.cd(mTargetDirectory);

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
