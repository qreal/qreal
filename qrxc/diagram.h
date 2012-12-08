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
	Diagram(QString const &name, QString const &nodeName, QString const &displayedName, Editor *editor);
	~Diagram();
	bool init(QDomElement const &diagramElement);
	bool resolve();
	Editor *editor() const;
	Type *findType(QString name);
	QMap<QString, Type*> types() const;
	QString name() const;
	QString nodeName() const;
	QString displayedName() const;
	QMap<QString, QStringList> paletteGroups() const;

private:
	struct ImportSpecification {
		QString name;
		QString as;
		QString displayedName;
	};

	QMap<QString, Type*> mTypes;
	QString mDiagramName;
	QString mDiagramNodeName;
	QString mDiagramDisplayedName;
	Editor *mEditor;
	QMap<QString, QStringList> mPaletteGroups;
	QList<ImportSpecification> mImports;

	bool initGraphicTypes(QDomElement const &graphicTypesElement);
	bool initNonGraphicTypes(QDomElement const &nonGraphicTypesElement);
	void initPaletteGroups(QDomElement const &paletteGroupsElement);
};
