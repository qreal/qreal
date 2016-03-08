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

/// Describes the metamodel of some visual langages.
/// The metamodel is a set of types of some entities associated each with other with some relations
/// (for example relation of generalization, relation of explosion, relation of containment, etc).
/// Metamodel thus is ordered multigraph where nodes are types of entities and edges are those relations.
/// This fact is noted with inheriting Metamodel from qrgraph::Multigraph.
class Metamodel : public qrgraph::Multigraph
{
public:
	/// Returns the name of this metamodel. This is internal and non-localized name and hence cannot be shown to user.
	virtual QString id() const = 0;

	/// Returns current version stamp of this metamodel. This version can be used for checking and providing
	/// backward compability with user saves written in editors of previous version.
	virtual QString version() const = 0;

	/// Returns a list of diagrams identifiers. Every element in this metamodel belongs to some diagram
	/// and only to one of them.
	virtual QStringList diagrams() const = 0;

	/// Returns a list of type descriptors of elements belonging to \a diagram. If metamodel does not contain
	/// \a diagram empty list will be returned.
	virtual QList<ElementType *> elements(const QString &diagram) const;

	//TODO ------------
	virtual QList<QPair<QPair<QString, QString>, QPair<bool, QString> > >
			getPossibleEdges(const QString &element) const = 0;
	//-----------------

	/// Returns type descriptor of the given element (the vertex of this multigraph with the given name).
	/// @warning If this metamodel does not contain \a diagram or \a diagram does not contain \a element
	/// then assertion fault will be generated.
	virtual ElementType &elementType(const QString &diagram, const QString &element) const;

	/// Returns a list of values of enum named \a name. First element of each pair in list is internal name of
	/// enum value used for its serialization and deserialization, it is language-independent. Second value is
	/// usually localized and can be used for displaying enum value to user.
	virtual QList<QPair<QString, QString>> enumValues(const QString &name) const = 0;

	/// If true is returned then enum \a name is editable. Editable enums can have arbitrary string as its value,
	/// but a list of enumValues() contains some kind of 'especial' values, they will be proposed to user
	/// when he edits enum value as auto-completion in combo box.
	virtual bool isEnumEditable(const QString &name) const = 0;

	/// Returns a string that can be shown to user as the name of this metamodel (it is often localized).
	virtual QString editorName() const = 0;

	/// Returns a string that can be shown to user as the name of a \a diagram (it is often localized).
	virtual QString diagramName(const QString &diagram) const = 0;

	/// Returns a type descriptor of the node or pattern element that should be created as root for the \a diagram.
	/// Nullptr can be returned if metamodel does not provide any element as root of \a diagram.
	virtual ElementType *diagramNode(const QString &diagram) const = 0;

	/// Returns a list of groups in palette for \a diagram. This list usually contains localized values,
	/// so they can be shown to user.
	virtual QStringList diagramPaletteGroups(const QString &diagram) const = 0;

	/// Returns a list of entries in palette \a group for \a diagram. The list contains type names of
	/// elements in \a group, patterns may belong to them.
	/// @todo: Change return value to QList<ElementType *> when patterns will generalize ElementType.
	virtual QStringList diagramPaletteGroupList(const QString &diagram, const QString &group) const = 0;

	/// Returns user-readable verbose string describing the logic of grouping entries in \a group.
	virtual QString diagramPaletteGroupDescription(const QString &diagram, const QString &group) const = 0;

	/// Returns true if entries in palette for \a diagram are expected to be sorted lexicographically.
	/// If false then the order of declarations in metamodel will be used.
	virtual bool shallPaletteBeSorted(const QString &diagram) const = 0;

protected:
	/// Can be called by subtypes to append new entity into this metamodel.
	/// @note Metamodel will take ownership on \a entity.
	void addNode(qrgraph::Node &entity) override;

	/// Can be called by subtypes to append new explosion types into this metamodel.
	/// @note Metamodel will take ownership in created explosion.
	void addExplosion(ElementType &source, ElementType &target, bool isReusable, bool requiresImmediateLinkage);

	QMap<QString, QMap<QString, ElementType *>> mElements;
};

}

Q_DECLARE_INTERFACE(qReal::Metamodel, "ru.spbsu.QReal.Metamodel/0.1")
