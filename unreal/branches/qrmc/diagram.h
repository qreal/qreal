#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>
#include <QtCore/QMap>
#include <QtCore/QPair>

#include "../../trunk/qrrepo/repoApi.h"

class Type;
class Editor;

class Diagram
{
public:
	Diagram(qReal::Id const &id, qrRepo::RepoApi *api, Editor *editor);
	~Diagram();
	bool init();
	bool resolve();
	Editor *editor() const;
	Type *findType(QString name);
	QMap<QString, Type*> types() const;
	QString name() const;
	QString nodeName() const;
	QString displayedName() const;

	QString generateNamesMap(QString const &namesTemplate);

	void print();

private:
	struct ImportSpecification {
		QString name;
		QString as;
		QString displayedName;
	};
	qReal::Id mId;
	qrRepo::RepoApi *mApi;
	QMap<QString, Type*> mTypes;
	QString mDiagramName;
	QString mDiagramNodeName; // TODO: replace with QStringList for multiple nodeNames
	QString mDiagramDisplayedName;
	Editor *mEditor;
	QList<ImportSpecification> mImports;
};
