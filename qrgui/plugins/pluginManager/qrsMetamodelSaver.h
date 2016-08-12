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
class PatternType;
enum class LinkShape;

/// An entity that saves metamodel into a .qrs-file containing meta-editor diagram describing metamodel.
/// Saved metamodel can then be loader with QrsMetamodelLoader.
class QRGUI_PLUGINS_MANAGER_EXPORT QrsMetamodelSaver : public QObject
{
	Q_OBJECT

public:
	/// Serializes \a metamodels into .qrs-file located at \a path.
	bool save(const QList<Metamodel *> &metamodels, const QString &path);

signals:
	/// Emitted when metamodel traversal encountered some error in metamodel.
	/// @param message Localized message that can be shown to user via error reporter.
	/// @param id The id of the element that contains this error, may be empty in general case.
	void errorOccured(const QString &message, const Id &id);

private:
	QString intVectorToString(const QVector<int> &values);

	QString penStyleToString(Qt::PenStyle style);
	QString linkShapeToString(LinkShape linkShape);

	void saveMetamodel(qrRepo::RepoApi &repo, const Metamodel &metamodel);
	void saveDiagram(qrRepo::RepoApi &repo, const Metamodel &metamodel
			, const QString &diagramName, const Id &metamodelDiagramId, Id &resultingId);
	void saveObjectsOnDiagram(qrRepo::RepoApi &repo, const Metamodel &metamodel
			, const QString &diagramName, const Id &diagram, QMap<const ElementType *, Id> &elements);
	Id saveNode(qrRepo::RepoApi &repo, const NodeElementType &node, const Id &diagram);
	Id saveEdge(qrRepo::RepoApi &repo, const EdgeElementType &edge, const Id &diagram);
	Id saveGroup(qrRepo::RepoApi &repo, const PatternType &pattern, const Id &diagram);
	void saveEnum(qrRepo::RepoApi &repo, const Metamodel &metamodel, const QString &enumName, const Id &diagram);
	void saveProperties(qrRepo::RepoApi &repo, const ElementType &element, const Id &id);
	void saveSdfGraphics(qrRepo::RepoApi &repo, const NodeElementType &node, const Id &id);
	void saveLabels(const NodeElementType &node, QDomElement &labelsTag, int width, int height);
	void saveNodePorts(const NodeElementType &node, QDomElement &portsTag);
	void saveContainerProperties(qrRepo::RepoApi &repo, const NodeElementType &node, const Id &id);
	void saveAssociations(qrRepo::RepoApi &repo, const EdgeElementType &edge, const Id &id);
	void savePorts(qrRepo::RepoApi &repo, const EdgeElementType &edge, const Id &id, const QString &direction);
	void saveEnumValues(qrRepo::RepoApi &repo, const Metamodel &metamodel, const QString &enumName, const Id &enumId);
	void saveGroupNodes(qrRepo::RepoApi &repo, const QDomElement &parent, const Id &id);
	void saveLinksInMetamodel(qrRepo::RepoApi &repo, const Metamodel &metamodel, const QString &diagramName
			, const Id &diagram, const QMap<const ElementType *, Id> &elements);
};

}
