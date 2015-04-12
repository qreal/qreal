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
		virtual QString generateElementDescription(const QString &lineTemplate) const = 0;//fix
		virtual QString generateReferenceProperties(const QString &lineTemplate) const = 0;
		virtual QString generatePortTypes(const QString &lineTemplate) const = 0;//oldfix
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
