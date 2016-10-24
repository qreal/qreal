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

#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QHash>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrrepo/logicalRepoApi.h>

namespace metaEditor {

/// Generates metamodel in .xml format to be compiled into C++ sources by qrxc tool
/// @todo Unify it with QrsMetamodelSerializer!
class EditorGenerator
{
public:
	explicit EditorGenerator(qrRepo::LogicalRepoApi const &api, qReal::ErrorReporterInterface &errorReporter);

	QHash<qReal::Id, QPair<QString, QString> > getMetamodelList();
	QPair<QString, QString> generateEditor(const qReal::Id &metamodelId, const QString &pathToFile
			, const QString &pathToQRealSource);

private:
	void serializeObjects(QDomElement &parent, const qReal::Id &idParent);
	void createImport(QDomElement &parent, const qReal::Id &id);
	void createNode(QDomElement &parent, const qReal::Id &id);
	void createEdge(QDomElement &parent, const qReal::Id &id);
	void createRole(QDomElement &parent, const qReal::Id &id);
	void createEnum(QDomElement &parent, const qReal::Id &id);
	void createPort(QDomElement &parent, const qReal::Id &id);
	void createGroup(QDomElement &parent, const qReal::Id &id);
	void setProperties(QDomElement &parent, const qReal::Id &id);
	void setPorts(QDomElement &parent, const qReal::Id &id, const QString &direction);
	void setValues(QDomElement &parent, const qReal::Id &id);
	void setGroupNodes(QDomElement &parent, const qReal::Id &id);
	void setUsages(QDomElement &parent, const qReal::Id &id);
	void setConnections(QDomElement &parent, const qReal::Id &id);
	void setGeneralization(QDomElement &parent, const qReal::Id &id);
	void setRoles(QDomElement &parent, const qReal::Id &id);
	void setPossibleEdges(QDomElement &parent, const qReal::Id &id);
	void setStatusElement(QDomElement &parent, const qReal::Id &id, const QString &tagName
			, const QString &propertyName);

	void setCreateChildrenFromMenu(QDomElement &parent, const qReal::Id &id);
	void createDiagrams(QDomElement &parent, const qReal::Id &id);
	void setContainer(QDomElement &parent, const qReal::Id &id);
	void setContainerProperties(QDomElement &parent, const qReal::Id &id);
	void setExplosion(QDomElement &parent, const qReal::Id &id);
	void setExplosionProperties(QDomElement &target, const qReal::Id &linkId);
	void setDividability(QDomElement &parent, const qReal::Id &id);
	void newSetConnections(QDomElement &parent, const qReal::Id &id
			, const QString &commonTagName, const QString &internalTagName, const QString &typeName);
	void ensureCorrectness(const qReal::Id &id, QDomElement element, const QString &tagName, const QString &value);
	bool findPort(const QString &name) const;
	void setBoolValuesForContainer(const QString &propertyName, QDomElement &properties, const qReal::Id &id);
	void setSizesForContainer(const QString &propertyName, QDomElement &properties, const qReal::Id &id);
	static void copyImages(const QString &pathToFile);
	static QString calculateEditorPath(const QString &pathToFile, const QString &pathToQRealSource);
	static QString calculateRelativeQRealSourcesPath(const QString &pathToFile, const QString &pathToQRealSource);
	static QString calculateRelativePath(const QString &pathOne, const QString &pathTwo);
	static void generateTranslations(const QString &path, const QString &name, const QString &qrealRoot);

	void checkRootNodeValid(const qReal::Id &diagram, const QString rootNode);

	qrRepo::LogicalRepoApi const &mApi;
	QDomDocument mDocument;
	QString mErrorText;
	qReal::IdList mElements;
	QString mDiagramName;
	qReal::ErrorReporterInterface &mErrorReporter;
};

}
