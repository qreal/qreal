#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>
#include <QtCore/QMap>
#include <QtCore/QPair>

class Type;
class Editor;

class Diagram
{
public:
	Diagram(QString const &name, Editor *editor);
	~Diagram();
	bool init(QDomElement const &diagramElement);
	bool resolve();
	Editor *editor() const;
	Type *findType(QString name);
	QMap<QString, Type*> types() const;
	QString name() const;

private:
	QMap<QString, Type*> mTypes;
	QString mDiagramName;
	Editor *mEditor;
	QList<QPair<QString, QString> > mImports;

	bool initGraphicTypes(QDomElement const &graphicTypesElement);
	bool initNonGraphicTypes(QDomElement const &nonGraphicTypesElement);
};
