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

#include <QtCore/QtPlugin>
#include <QtGui/QIcon>

#include <qrgraph/multigraph.h>

#include "metaMetaModel/elementType.h"

namespace qReal {

/// Describes the metamodel of some visual languages.
/// The metamodel is a set of types of some entities associated each with other with some relations
/// (for example relation of generalization, relation of explosion, relation of containment, etc).
/// Metamodel thus is ordered multigraph where nodes are types of entities and edges are those relations.
class QRGUI_META_META_MODEL_EXPORT Metamodel : public qrgraph::Multigraph
{
public:
	Metamodel();

	/// Returns the name of this metamodel. This is internal and non-localized name and hence cannot be shown to user.
	QString id() const;

	/// Sets the internal and non-localized name of this metamodel that will be used as first part of elements ids.
	void setId(const QString &id);

	/// Returns current version stamp of this metamodel. This version can be used for checking and providing
	/// backward compability with user saves written in editors of previous version.
	QString version() const;

	/// Sets the current version stamp of this metamodel.
	void setVersion(const QString &version);

	/// Returns a list of diagrams identifiers. Every element in this metamodel belongs to some diagram
	/// and only to one of them.
	const QStringList &diagrams() const;

	/// Sets a list of diagrams identifiers. Every element in this metamodel must belong to some diagram
	/// and only to one of them.
	void setDiagrams(const QStringList &diagrams);

	/// Appends \a diagramName to a list of diagrams identifiers.
	void addDiagram(const QString &diagramName);

	/// Returns a list of type descriptors of elements belonging to \a diagram. If metamodel does not contain
	/// \a diagram empty list will be returned.
	QList<ElementType *> elements(const QString &diagram) const;

	/// Can be called to append new entity into this metamodel.
	/// @note Metamodel will take ownership on \a entity.
	void addNode(qrgraph::Node &entity) override;

	/// Returns type descriptor of the given element (the vertex of this multigraph with the given name).
	/// @warning If this metamodel does not contain \a diagram or \a diagram does not contain \a element
	/// then assertion fault will be generated.
	ElementType &elementType(const Id &id) const;

	/// Returns type descriptor of the given element (the vertex of this multigraph with the given name).
	/// @warning If this metamodel does not contain \a diagram or \a diagram does not contain \a element
	/// then assertion fault will be generated.
	ElementType &elementType(const QString &diagram, const QString &element) const;

	/// Appends new entity into this metamodel.
	void addElement(ElementType &element);

	/// Returns names of enum types provided by this metamodel.
	QStringList enumNames() const;

	/// Returns a list of values of enum named \a name. First element of each pair in list is internal name of
	/// enum value used for its serialization and deserialization, it is language-independent. Second value is
	/// usually localized and can be used for displaying enum value to user.
	QList<QPair<QString, QString>> enumValues(const QString &name) const;

	/// Appends enum to a list of values of enum named \a name. First element of each pair in list is internal name of
	/// enum value used for its serialization and deserialization, it is language-independent. Second value is
	/// usually localized and can be used for displaying enum value to user.
	void addEnum(const QString &name, const QList<QPair<QString, QString>> &values);

	/// If true is returned then enum \a name is editable. Editable enums can have arbitrary string as its value,
	/// but a list of enumValues() contains some kind of 'especial' values, they will be proposed to user
	/// when he edits enum value as auto-completion in combo box.
	bool isEnumEditable(const QString &name) const;

	/// Switches editability for enum with the name \a name. Editable enums can have arbitrary string as its value,
	/// but a list of enumValues() contains some kind of 'especial' values, they will be proposed to user
	/// when he edits enum value as auto-completion in combo box.
	void setEnumEditable(const QString &name, bool editable);

	/// Returns a string that can be shown to user as the name of this metamodel (it is often localized).
	QString friendlyName() const;

	/// Sets a string that can be shown to user as the name of this metamodel (it should be localized).
	void setFriendlyName(const QString &friendlyName);

	/// Returns a string that can be shown to user as the name of a \a diagram (it is often localized).
	QString diagramFriendlyName(const QString &diagram) const;

	/// Sets a string that can be shown to user as the name of a \a diagram (it should be localized).
	void setDiagramFriendlyName(const QString &diagram, const QString &friendlyName);

	/// Returns a type descriptor of the node or pattern element that should be created as root for the \a diagram.
	/// Nullptr can be returned if metamodel does not provide any element as root of \a diagram.
	ElementType *diagramNode(const QString &diagram) const;

	/// Makes a given node or pattern element automaticly created as root for the \a diagram.
	/// Nullptr can be returned if metamodel does not provide any element as root of \a diagram.
	void setDiagramNode(const QString &diagram, const QString &elementName);

	/// Returns a list of groups in palette for \a diagram. This list usually contains localized values,
	/// so they can be shown to user.
	QStringList diagramPaletteGroups(const QString &diagram) const;

	/// Appends \a group a list of groups in palette for \a diagram. \a group should be localized,
	/// so that it can be shown to user.
	void appendDiagramPaletteGroup(const QString &diagram, const QString &group);

	/// Returns a list of entries in palette \a group for \a diagram. The list contains type names of
	/// elements in \a group, patterns may belong to them.
	QStringList diagramPaletteGroupList(const QString &diagram, const QString &group) const;

	/// Adds \a element into a palette \a group of the \a diagram.
	void addElementToDiagramPaletteGroup(const QString &diagram, const QString &group, const QString &element);

	/// Returns user-readable verbose string describing the logic of grouping entries in \a group.
	QString diagramPaletteGroupDescription(const QString &diagram, const QString &group) const;

	/// Sets user-readable verbose string describing the logic of grouping entries in \a group.
	void setDiagramPaletteGroupDescription(const QString &diagram, const QString &group, const QString &description);

	/// Returns true if entries in palette for \a diagram are expected to be sorted lexicographically.
	/// If false then the order of declarations in metamodel will be used.
	bool shallPaletteBeSorted(const QString &diagram) const;

	/// Turns on or off lexicographical sorting of entries in palette for \a diagram.
	/// If \a sorted is false then the order of declarations in metamodel will be used.
	void setPaletteSorted(const QString &diagram, bool sorted);

	/// Can be called by subtypes to append new explosion types into this metamodel.
	/// @note Metamodel will take ownership in created explosion.
	void addExplosion(ElementType &source, ElementType &target, bool isReusable, bool requiresImmediateLinkage);

private:
	QString mId;
	QString mVersion;
	QStringList mDiagrams;
	QString mFriendlyName;
	QMap<QString, QMap<QString, ElementType *>> mElements;
	QMap<QString, QList<QPair<QString, QString>>> mEnumValues;
	QMap<QString, QString> mEnumDisplayedNames;
	QMap<QString, bool> mEnumsEditability;
	QMap<QString, QString> mDiagramFriendlyNames;
	QMap<QString, QString> mDiagramNodes;
	QMap<QString, QStringList> mPaletteGroups;
	QMap<QString, QMap<QString, QStringList>> mPaletteGroupContents;
	QMap<QString, QMap<QString, QString>> mPaletteGroupDescriptions;
	QMap<QString, bool> mPaletteSorting;
};

/// An interface for all objects that load information into metamodel.
class MetamodelLoaderInterface
{
public:
	virtual ~MetamodelLoaderInterface() {}

	/// Will be implemented to return the a list of names of metamodels that should be loaded before this one.
	/// @todo: This should be an information in plugin metadata and be processed by common code in plugin manager.
	virtual QStringList dependencies() const = 0;

	/// Will be implemented to fill or extend \a metamodel's data.
	virtual void load(Metamodel &metamodel) = 0;
};

}

Q_DECLARE_INTERFACE(qReal::Metamodel, "ru.spbsu.QReal.Metamodel/0.1")
Q_DECLARE_INTERFACE(qReal::MetamodelLoaderInterface, "ru.spbsu.QReal.MetamodelLoaderInterface/0.1")
