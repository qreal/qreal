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

#include <QtCore/QMap>
#include <QtCore/QString>

#include "../utils/defs.h"
#include "../../qrrepo/repoApi.h"

namespace qrmc {

	class Property;
	class Diagram;


	class Type
	{
	public:
		Type(bool isResolved, Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id);
		virtual ~Type();
		virtual Type* clone() const = 0;
		virtual bool resolve() = 0;
		virtual bool init(const QString &context);
		virtual bool isResolving() const;
		virtual bool isGraphicalType() const = 0;
		virtual bool isResolved() const;

		virtual void print() = 0;

		virtual QString name() const;
		virtual QString path() const;
		virtual QString qualifiedName() const;
		virtual QString displayedName() const;
		virtual QString nativeContext() const;

		virtual Diagram *diagram() const;

		virtual QMap<QString, Property*> properties() const;

		virtual void setName(const QString &name);
		virtual void setDiagram(Diagram *diagram);
		virtual void setContext(const QString &newContext);
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
		virtual void copyFields(Type *type) const;

		QMap<QString, Property*> mProperties;
		bool mResolvingFinished;
		Diagram *mDiagram;
		qReal::Id mId;
		qrRepo::LogicalRepoApi *mApi;
		QString mName;  // metatype name
		QString mContext;  // context if metatype. e.g. Kernel::Node: Node - name, Kernel - context.
		QString mNativeContext;  // native context, doesn't change on import and is used for element resolving
		QString mDisplayedName;
		QString mPath;
	};
}
