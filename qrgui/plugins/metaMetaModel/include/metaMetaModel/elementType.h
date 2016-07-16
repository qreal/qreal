/* Copyright 2016 CyberTech Labs Ltd, Dmitry Mordvinov
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

#include <qrkernel/ids.h>
#include <qrgraph/node.h>

#include "metaMetaModel/labelProperties.h"
#include "metaMetaModel/explosion.h"

class QDomDocument;
class QDomElement;

namespace qReal {

class Metamodel;
class NodeElementType;
class EdgeElementType;
class PatternType;

/// Describes type of elements that can be instantiated on scene.
/// The metamodel of some visual language is a number of such types associated each with other with some relations
/// (for example relation of generalization, relation of explosion, relation of containment, etc).
/// Metamodel thus is ordered multigraph where nodes are types of elements.
/// This fact is noted with inheriting ElementType from qrgraph::Node.
class QRGUI_META_META_MODEL_EXPORT ElementType : public qrgraph::Node
{
public:
	// Types of links in metamodel go below, may be extended:
	static const uint generalizationLinkType = 0;
	static const uint containmentLinkType = 1;
	static const uint explosionLinkType = 2;

	/// Enumerates types of entities that can be dragged out of a palette. All those enities have type descriptors
	/// subclassing this class.
	enum class Type
	{
		/// This type describes node element, toNode() can be safely called.
		node = 0
		/// This type describes edge element, toEdge() can be safely called.
		, edge
		/// This type describes group of nodes and edges, toPattern() can be safely called.
		, pattern
	};

	~ElementType() override;

	/// Returns a reference to instance of metamodel this type belogns to.
	Metamodel &metamodel() const;

	/// Casts this instance to node element type. If this is not node then assertion fault will be generated.
	/// @see type().
	NodeElementType &toNode();

	/// Casts this instance to node element type. If this is not node then assertion fault will be generated.
	/// @see type().
	const NodeElementType &toNode() const;

	/// Casts this instance to edge element type. If this is not edge then assertion fault will be generated.
	/// @see type().
	EdgeElementType &toEdge();

	/// Casts this instance to edge element type. If this is not edge then assertion fault will be generated.
	/// @see type().
	const EdgeElementType &toEdge() const;

	/// Casts this instance to pattern element type. If this is not pattern then assertion fault will be generated.
	/// @see type().
	PatternType &toPattern();

	/// Casts this instance to pattern element type. If this is not pattern then assertion fault will be generated.
	/// @see type().
	const PatternType &toPattern() const;

	/// Returns the name of metamodel that can be used as first part of qReal::Id.
	const QString editor() const;

	/// Returns hierarchical name of this type expressed as qReal::Id.
	const Id typeId() const;

	/// Returns true if this element type generalizes \a parent. Distant
	virtual bool isParent(const ElementType &parent) const;

	/// Returns true if this element type generalizes \a parent. Distant
	virtual IdList containedTypes() const;

	/// Returns a list of explosions that may be outgoing from instances of this type.
	virtual QList<const Explosion *> explosions() const;

	/// Returns true if this instance describes node element type.
	virtual Type type() const = 0;

	/// Returns the internal name if this element. This name can be used for element() part of qReal::Id and should
	/// not be shown to user.
	QString name() const;

	/// Sets the internal name if this element. This name can be used for element() part of qReal::Id and should
	/// not be shown to user.
	void setName(const QString &name);

	/// Returns the localized name assigned to instances of this type by default.
	QString friendlyName() const;

	/// Sets the localized name assigned to instances of this type by default.
	void setFriendlyName(const QString &friendlyName);

	/// Returns localized string describing this type. This string can be shown to user.
	QString description() const;

	/// Sets localized string describing this type. This string will be shown to user.
	void setDescription(const QString &description);

	/// Returns the internal name of the diagram this element belongs to.
	/// This name can be used for diagram() part of qReal::Id and should not be shown to user.
	QString diagram() const;

	/// Sets the internal name of the diagram this element belongs to.
	/// This name will be used for diagram() part of qReal::Id and should not be shown to user.
	void setDiagram(const QString &diagramName);

	/// Returns XML element containing SDF description of element's appearance.
	QDomElement sdf() const;

	/// Loads SDF description of element's appearance.
	void loadSdf(const QDomElement &picture);

	/// Returns a list of all labels on instances of this type.
	const QList<LabelProperties> &labels() const;

	/// Appends \a label to a list of all labels belonging to instances of this type.
	void addLabel(const LabelProperties &label);

	/// Returns a list of custom logical properties of this element.
	const QStringList &propertyNames() const;

	/// Returns a list of reference properties of this element.
	/// Reference properties are those whoose value is another element.
	const QStringList &referenceProperties() const;

	/// Returns a list of property names that were removed by user in 'metamodeling-on-fly' mode.
	const QStringList &hiddenProperties() const;

	/// Returns a type name of a property with the given \a name.
	QString propertyType(const QString &name) const;

	/// Returns a string representation of a value assigned to \a property by default.
	QString propertyDefaultValue(const QString &property) const;

	/// Returns localized string describing some \a property. This string can be shown to user.
	QString propertyDescription(const QString &property) const;

	/// Returns localized name of some \a property. The resulting string can be shown to user.
	QString propertyDisplayedName(const QString &property) const;

	/// Appends property with the \a name to a list of properties of this type.
	/// @param name The name of new property, if same name already contained by this type it will be overloaded.
	/// @param type Type name of the property, for example 'int' or 'string' or some enum`s name.
	/// @param defaultValue String representation of value assigned to this property by default.
	/// @param displayedName A string that will be shown to user as a name of this property, should be localized.
	/// @param description A string that will be shown to user as a description of this property, should be localized.
	/// @param isReference Must be true for properties whoose values point to some other element.
	void addProperty(const QString &name, const QString &type, const QString &defaultValue
			, const QString &displayedName, const QString &description, bool isReference);

	/// Returns true if this element type was removed by user in 'metamodeling-on-fly' mode.
	bool isHidden() const;

	/// Removes or restores this element from metamodel.
	void setHidden(bool isHidden);

protected:
	/// @param metamodel Metamodel that owns this element.
	explicit ElementType(Metamodel &metamodel);

private:
	QString mName;
	QString mFriendlyName;
	QString mDescription;
	QString mDiagram;
	QScopedPointer<QDomDocument> mSdf;
	QList<LabelProperties> mLabels;
	QStringList mPropertyNames;
	QStringList mReferenceProperties;
	QStringList mHiddenProperties;
	QMap<QString, QString> mPropertyTypes;
	QMap<QString, QString> mPropertyDefaultValues;
	QMap<QString, QString> mPropertyDescriptions;
	QMap<QString, QString> mPropertyDisplayedNames;
	bool mIsHidden;
};

}
