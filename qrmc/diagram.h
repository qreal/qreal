/* Copyright 2007-2015 QReal Research Group, Yurii Litvinov
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

#include <functional>

#include <QtCore/QString>
#include <QtCore/QMap>

#include <qrkernel/ids.h>

namespace qrRepo {
class LogicalRepoApi;
}

namespace qrmc {

class Type;
class Editor;

/// Represents a diagram --- tab in the palette, or one visual language.
class Diagram
{
public:
	/// Constructor.
	/// @param id - id of a diagram.
	/// @param api - repository with metamodel.
	/// @editor - editor to which this diagram belongs to.
	/// @targetDirectory - directory to which the code shall be generated.
	Diagram(const qReal::Id &id, const qrRepo::LogicalRepoApi &api, Editor &editor
			, const QString &targetDirectory);

	~Diagram();

	/// Loads diagram from metamodel.
	bool init();

	/// Resolves imported types, processes generalization.
	bool resolve();

	/// Returns editor to which this diagram belongs to.
	Editor *editor() const;

	/// Returns type with given name or nullptr if there is no type in this diagram.
	Type *findType(const QString &name) const;

	/// Returns all types in this diagram.
	const QMap<QString, Type*> &types() const;

	/// Returns internal name of the diagram.
	QString name() const;

	/// Returns name of the root element of this diagram.
	QString nodeName() const;

	/// Returns name of a diagram as it is displayed to user.
	QString displayedName() const;

	QString generateNamesMap(const QString &lineTemplate) const;
	QString generateMouseGesturesMap(const QString &lineTemplate) const;
	QString generatePropertiesMap(const QString &lineTemplate) const;
	QString generatePropertyDefaultsMap(const QString &lineTemplate) const;
	QString generatePropertyDisplayedNamesMap(const QString &lineTemplate) const;
	QString generateElementDescriptionMap(const QString &lineTemplate) const;
	QString generateParentsMap(const QString &lineTemplate) const;
	QString generateContainers(const QString &lineTemplate) const;
	QString generateReferenceProperties(const QString &lineTemplate) const;
	QString generatePortTypes(const QString &lineTemplate) const;
	QString generatePropertyName(const QString &lineTemplate) const;
	QString generateConnections(const QString &lineTemplate) const;
	QString generateUsages(const QString &lineTemplate) const;
	QString generateFactory(const QString &lineTemplate) const;
	QString generateIsNodeOrEdge(const QString &lineTemplate) const;
	QString generateEnums(const QString &lineTemplate) const;
	QString generatePossibleEdges(const QString &lineTemplate) const;

	QString generateNodeClasses(const QString &nodeTemplate) const;
	QString generateEdgeClasses(const QString &edgeTemplate) const;

	QString generateResourceFile(const QString &resourceTemplate) const;

private:
	struct ImportSpecification {
		QString name;
		QString as;
		QString displayedName;
	};

	const qReal::Id mId;
	const qrRepo::LogicalRepoApi &mApi;

	/// Has ownership.
	QMap<QString, Type*> mTypes;

	QString mDiagramName;
	QString mDiagramNodeName; // TODO: replace with QStringList for multiple nodeNames
	QString mDiagramDisplayedName;
	Editor &mEditor;
	QList<ImportSpecification> mImports;
	const QString mTargetDirectory;

	QString generateListMethod(const QString &lineTemplate
			, std::function<QString(const Type * const, const QString &)> generator) const;

	QString generateMapMethod(const QString& lineTemplate
			, std::function<QString(Type * const, const QString &)> generator) const;
};

}
