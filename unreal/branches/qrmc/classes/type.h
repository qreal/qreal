#pragma once

#include <QtCore/QMap>
#include <QtCore/QString>

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
	bool isResolved() const;

	virtual void print() = 0;

	QString name() const;
	QString path() const;
	QString qualifiedName() const;
	QString displayedName() const;

	QMap<QString, Property*> properties() const;

	void setName(QString const &name);
	void setDiagram(Diagram *diagram);
	void setContext(QString const &newContext);
	void setDisplayedName(QString const &displayedName);

protected:
	void copyFields(Type *type) const;
	QString nativeContext() const;

	QMap<QString, Property*> mProperties;
	bool mResolvingFinished;
	Diagram *mDiagram;
	qReal::Id mId;
	qrRepo::RepoApi *mApi;
	QString mName;  // Неквалифицированное имя метатипа
	QString mContext;  // Контекст квалификации. Например, для Kernel::Node: Node - имя, Kernel - контекст.
	QString mNativeContext;  // "� одной" контекст квалификации, не меняется при импорте типа и используется для ресолва.
	QString mDisplayedName;
	QString mPath;
};
