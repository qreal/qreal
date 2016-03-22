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

#pragma once

#include <functional>

#include <qrkernel/ids.h>

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QSet>

namespace qrRepo {
class LogicalRepoApi;
}

namespace qrmc {
class MetaCompiler;
class Diagram;
class Type;
class EnumType;

namespace utils {
class OutFile;
}

/// Represents one editor (a set of diagrams) loaded from metamodel.
/// @todo: make it compliant with global project dictionary. Editor is actually Metamodel.
class Editor
{
public:
	/// Constructor.
	Editor(MetaCompiler &metaCompiler, const qrRepo::LogicalRepoApi &api
			, const qReal::Id &id, const QString &targetDirectory);

	~Editor();

	/// Returns metacompiler object that has templates and tools used by this editor.
	MetaCompiler &metaCompiler();

	/// Id of this editor node in metamodel.
	qReal::Id id() const;

	/// Returns true if this editor is already loaded.
	bool isLoaded() const;

	/// Loads this editor: loads included metamodels, loads diagrams in this metamodel, resolves metatypes.
	bool load();

	/// Generates loaded editor. Takes in a bunch of templates and generates a bunch of C++ files with editor
	/// implementation.
	void generate(const QString &headerTemplate, const QString &sourceTemplate,
			const QString &nodeTemplate, const QString &edgeTemplate,
			const QString &elementsHeaderTemplate, const QString &resourceTemplate,
			const QString &projectTemplate, const QMap<QString, QString> &utils);

	/// Returns type with given name in this (or included) editor or nullptr if there is no such type.
	Type *findType(const QString &name) const;

	/// Returns all enums in this and included editors.
	QSet<EnumType*> getAllEnumTypes() const;

	/// Returns a list of all port names in this and included editors, whatever the hell it is.
	QStringList getAllPortNames() const;

	/// Returns diagram by given name or nullptr if there is no such diagram in this editor.
	Diagram *findDiagram(const QString &name) const;

	/// Returns diagrams in this editor.
	QMap<QString, Diagram*> diagrams() const;

	/// Returns name of this editor.
	QString name() const;

private:
	bool generatePluginHeader(const QString &headerTemplate);
	bool generatePluginSource();
	bool generateElementsClasses();
	bool generateResourceFile(const QString &resourceTemplate);
	bool generateProjectFile(const QString &projectTemplate);

	void generateDiagramsMap();
	void generateDiagramNodeNamesMap();
	void generateEnums();

	/// Loads all metamodels referenced from this metamodel which are present on the same save. After this mIncludes
	/// field will be filled with loaded diagrams.
	/// @returns true if successful.
	bool loadIncludes();

	/// Loads diagrams in this metamodel, but does not resolve. After this mDiagrams field will be filled with loaded
	/// diagrams.
	/// @returns true, if successful.
	bool loadDiagrams();

	/// Resolves types in loaded diagrams. After this mDiagrams will be filled with resolved diagrams.
	/// @returns true, if successful.
	bool resolve();

	/// Generates one method for plugin implementaion using specified template tag and specified generator.
	void generatePluginMethod(const QString &tag, const std::function<QString(Diagram *, const QString &)> &generator);

	MetaCompiler &mMetaCompiler;
	const qrRepo::LogicalRepoApi &mApi;
	const qReal::Id mId;
	QString mName;
	QString mNameOfMetamodel;
	bool mLoadingComplete = false;

	/// Does not has ownership.
	QList<Editor*> mIncludes;

	/// Has ownership.
	QMap<QString, Diagram*> mDiagrams;

	QMap<QString, QString> mUtilsTemplate;
	QString mSourceTemplate;
	QString mNodeTemplate;
	QString mEdgeTemplate;
	QString mElementsHeaderTemplate;
	const QString mTargetDirectory;
};

}
