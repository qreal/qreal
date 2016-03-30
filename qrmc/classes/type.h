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

#include <QtCore/QMap>
#include <QtCore/QString>

#include "qrmc/utils/defs.h"
#include <qrrepo/repoApi.h>

namespace qrmc {

class Property;
class Diagram;

/// Virtual base class of all metatypes supported by metaeditor.
class Type
{
public:
	/// Constructor.
	/// @param isResolved - true, if type is already resolved.
	/// @param diagram - diagram to which this type belongs to.
	/// @param api - repository with metamodel.
	/// @param id - id of a type in repository.
	Type(bool isResolved, const Diagram &diagram, const qrRepo::LogicalRepoApi &api, const qReal::Id &id);

	virtual ~Type();

	/// Provides copy of this type.
	/// Ownership passed to the caller.
	virtual Type* clone() const = 0;

	/// Resolves inheritance by copying properties of a parent to this type.
	virtual bool resolve() = 0;

	/// Initializes this type in a given context (context is like namespace).
	virtual bool init(const QString &context);

	/// Returns true if this type is currently in "resolving" status.
	virtual bool isResolving() const;

	/// Returns true if this type is graphical (i.e. can be drawn by editor).
	virtual bool isGraphicalType() const = 0;

	/// Returns true if this type is already resolved.
	virtual bool isResolved() const;

	/// Debug print method.
	virtual void print() = 0;

	/// Returns name of the type.
	virtual QString name() const;

	/// Returns mouse gesture path of this type.
	virtual QString path() const;

	/// Returns fully qualified name (context + name).
	virtual QString qualifiedName() const;

	/// Returns displayed name of a type.
	virtual QString displayedName() const;

	/// Returns context to which this type belongs to originally (may differ from context when type was copied
	/// by "include").
	virtual QString nativeContext() const;

	/// Returns diagram this type belongs to.
	virtual const Diagram &diagram() const;

	/// Returns all properties of this type. Does not transfer ownership.
	virtual const QMap<QString, Property*> &properties() const;

	/// Sets a name of this type.
	virtual void setName(const QString &name);

	/// Sets a diagram this type belongs to.
	virtual void setDiagram(Diagram &diagram);

	/// Sets new context for this type.
	virtual void setContext(const QString &newContext);

	/// Sets displayed name of this type.
	virtual void setDisplayedName(const QString &displayedName);

	virtual QString generateNames(const QString &lineTemplate) const;
	virtual QString generateMouseGestures(const QString &lineTemplate) const;
	virtual QString generateProperties(const QString &lineTemplate) const = 0;
	virtual QString generatePropertyDefaults(const QString &lineTemplate) const = 0;
	virtual QString generatePropertyDisplayedNames(const QString &lineTemplate) const = 0;
	virtual QString generateElementDescription(const QString &lineTemplate) const = 0;
	virtual QString generateReferenceProperties(const QString &lineTemplate) const = 0;
	virtual QString generatePortTypes(const QString &lineTemplate) const = 0;
	virtual QString generatePropertyName(const QString &lineTemplate) const = 0;
	virtual QString generateParents(const QString &lineTemplate) const = 0;
	virtual QString generateContainers(const QString &lineTemplate) const = 0;
	virtual QString generateConnections(const QString &lineTemplate) const = 0;
	virtual QString generateUsages(const QString &lineTemplate) const = 0;
	virtual QString generateFactory(const QString &lineTemplate) const;
	virtual QString generateIsNodeOrEdge(const QString &lineTemplate) const = 0;
	virtual QString generateEnums(const QString &lineTemplate) const = 0;
	virtual QString generatePossibleEdges(const QString &lineTemplate) const = 0;

	virtual QString generateNodeClass(const QString &classTemplate) = 0;
	virtual QString generateEdgeClass(const QString &classTemplate) const = 0;
	virtual QString generateResourceLine(const QString &classTemplate) const = 0;

protected:
	/// Copies all fields of this type to a given type.
	virtual void copyFields(Type *type) const;

	/// Map of all properties of a type.
	/// Has ownership.
	QMap<QString, Property*> mProperties;

	/// Flag that becomes true if this type is resolved.
	bool mResolvingFinished = false;

	/// Diagram this type belongs to.
	/// Does not have ownership.
	const Diagram *mDiagram;

	/// Id of this type in repository.
	const qReal::Id mId;

	/// Repository with metamodel.
	const qrRepo::LogicalRepoApi &mApi;

	/// Metatype name.
	QString mName;

	/// Context if metatype. e.g. Kernel::Node: Node - name, Kernel - context.
	QString mContext;

	/// Native context, doesn't change on import and is used for element resolving.
	QString mNativeContext;

	/// Name of this type as it is displayed to user.
	QString mDisplayedName;

	/// Mouse gesture path.
	QString mPath;
};

}
