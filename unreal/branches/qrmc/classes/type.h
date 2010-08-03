#pragma once

#include <QtCore/QMap>
#include <QtCore/QString>

#include "utils/defs.h"
#include "../../../trunk/qrrepo/repoApi.h"

class Property;
class Diagram;

namespace utils {
	class OutFile;
}

class Type
{
public:
	Type(bool isResolved, Diagram *diagram, qrRepo::RepoApi *api, qReal::Id const &id);
	virtual ~Type();
	virtual Type* clone() const = 0;
	virtual bool resolve() = 0;
	virtual bool init(QString const &context);
	virtual bool isResolving() const;
	virtual bool isGraphicalType() const = 0;
	virtual bool isResolved() const;

	virtual void print() = 0;

	virtual QString name() const;
	virtual QString path() const;
	virtual QString qualifiedName() const;
	virtual QString displayedName() const;

	virtual Diagram *diagram() const;

	virtual QMap<QString, Property*> properties() const;

	virtual void setName(QString const &name);
	virtual void setDiagram(Diagram *diagram);
	virtual void setContext(QString const &newContext);
	virtual void setDisplayedName(QString const &displayedName);

	virtual QString generateNames(QString const &lineTemplate) const;
	virtual QString generateMouseGestures(QString const &lineTemplate) const;
	virtual QString generateProperties(QString const &lineTemplate) const = 0;
	virtual QString generatePropertyDefaults(QString const &lineTemplate) const = 0;
	virtual QString generateContainers(QString const &lineTemplate) const = 0;
	virtual QString generateConnections(QString const &lineTemplate) const = 0;
	virtual QString generateUsages(QString const &lineTemplate) const = 0;
	virtual QString generateFactory(QString const &lineTemplate) const;
	virtual QString generateIsNodeOrEdge(QString const &lineTemplate) const = 0;
	virtual QString generateEnums(QString const &lineTemplate) const = 0;
	virtual QString generatePossibleEdges(QString const &lineTemplate) const = 0;

	virtual QString generateNodeClass(QString const &classTemplate) const = 0;
	virtual QString generateEdgeClass(QString const &classTemplate) const = 0;

protected:
	virtual void copyFields(Type *type) const;
	virtual QString nativeContext() const;

	QMap<QString, Property*> mProperties;
	bool mResolvingFinished;
	Diagram *mDiagram;
	qReal::Id mId;
	qrRepo::RepoApi *mApi;
	QString mName;  // metatype name
	QString mContext;  // context if metatype. e.g. Kernel::Node: Node - name, Kernel - context.
	QString mNativeContext;  // native context, doesn't change on import and is used for element resolving
	QString mDisplayedName;
	QString mPath;
};
