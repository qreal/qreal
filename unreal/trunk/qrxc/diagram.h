#pragma once

#include <QString>
#include <QDomElement>
#include <QMap>

class Type;
class Editor;

class Diagram
{
public:
	Diagram(QString const &name, Editor *editor);
	~Diagram();
	bool init(QDomElement const &diagramElement);
	bool resolve();
	Editor *editor();
	Type *findType(QString name);
	QMap<QString, Type*> types();
	QString name();

private:
	QMap<QString, Type*> mTypes;
	QString mDiagramName;
	Editor *mEditor;

	bool initGraphicTypes(QDomElement const &graphicTypesElement);
	bool initNonGraphicTypes(QDomElement const &nonGraphicTypesElement);
};
