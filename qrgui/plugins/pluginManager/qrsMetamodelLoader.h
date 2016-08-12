/* Copyright 2016 QReal Research Group, Dmitry Mordvinov
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

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "pluginsManagerDeclSpec.h"

class QDomElement;

namespace qrRepo {
class RepoApi;
}

namespace qReal {

class Id;
class Metamodel;
class ElementType;
class NodeElementType;
class EdgeElementType;
enum class LinkShape;

/// An entity that constructs metamodel from description in meta-editor diagram.
/// Loaded metamodel can then be saver with QrsMetamodelSaver.
class QRGUI_PLUGINS_MANAGER_EXPORT QrsMetamodelLoader : public QObject
{
	Q_OBJECT

public:
	/// Opens and parses .qrs save file containing serialized metamodel and creates Metamodel object instance
	/// from it. If something went wrong errorOccured() will be emitted. Resulting list may be empty.
	/// @note Transfers owership to caller.
	QList<Metamodel *> load(const QString &pathToQrs);

signals:
	/// Emitted when metamodel parser found some error in metamodel.
	/// @param message Localized message that can be shown to user via error reporter.
	/// @param id The id of the element that contains this error, may be empty in general case.
	void errorOccured(const QString &message, const Id &id);

private:
	QString stringProperty(const qrRepo::RepoApi &repo, const Id &id
			, const QString &propertyName, const QString &defaultValue = QString());
	int parseInt(const QString &string, const Id &id);
	int intProperty(const qrRepo::RepoApi &repo, const Id &id, const QString &propertyName, int defaultValue = 0);
	QVector<int> intVectorProperty(const qrRepo::RepoApi &repo, const Id &id
			, const QString &propertyName, const QVector<int> &defaultValue = QVector<int>());
	bool boolProperty(const qrRepo::RepoApi &repo, const Id &id
			, const QString &propertyName, bool defaultValue = false);

	Qt::PenStyle parsePenStyle(const QString &penStyle, const Id &id);
	LinkShape parseLinkShape(const QString &linkShape, const Id &id);

	QString validateName(const qrRepo::RepoApi &repo, const Id &id);
	QString validateRootNode(const qrRepo::RepoApi &repo, const Id &diagram);
	QString validatePortName(const qrRepo::RepoApi &repo, const Id &id, const QString &name);

	Metamodel *parseMetamodel(const qrRepo::RepoApi &repo, const Id &metamodelDiagram);
	void parseDiagram(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram);
	void parseObjectsOnDiagram(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram);
	void parseImport(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id);
	void parseNode(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram, const Id &id);
	void parseEdge(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram, const Id &id);
	void parseEnum(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id);
	void parsePort(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id);
	void parseGroup(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram, const Id &id);
	void parseProperties(const qrRepo::RepoApi &repo, ElementType &element, const Id &id);
	void parseSdfGraphics(const qrRepo::RepoApi &repo, NodeElementType &node, const Id &id);
	void parseLabels(NodeElementType &node, const QDomElement &labelsTag, int width, int height);
	void parseNodePorts(NodeElementType &node, const QDomElement &portsTag, int width, int height);
	void parseContainerProperties(const qrRepo::RepoApi &repo, NodeElementType &node, const Id &id);
	void parseAssociations(const qrRepo::RepoApi &repo, EdgeElementType &edge, const Id &id);
	void parsePorts(const qrRepo::RepoApi &repo, EdgeElementType &edge, const Id &id, const QString &direction);
	QList<QPair<QString, QString>> parseEnumValues(const qrRepo::RepoApi &repo, const Id &id);
	void parseGroupNodes(const qrRepo::RepoApi &repo, QDomElement &parent, const Id &id);
	void parseLinksOnDiagram(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram);
	void parseGeneralization(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id, const QString &diagram
			, ElementType *&fromElement, ElementType *&toElement, QString &overridingProperties);
	void parseContainer(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id, const QString &diagram);
	void parseExplosion(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id, const QString &diagram);

	void resolveInheritance(QSet<ElementType *> &elements
			, const QHash<QPair<ElementType *, ElementType *>, QString> &overridingProperties);
	void inherit(ElementType &child, const ElementType &parent, const QString &propreties);
};

}
