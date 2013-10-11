#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>
#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QStringList>

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
	QStringList subDiagramNames() const;
	QString displayedName() const;
	QString getGroupsXML() const;
	QMap<QString, QStringList> paletteGroups() const;
	QMap<QString, QString> paletteGroupsDescriptions() const;

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
	QString mGroupsXML;
	QMap<QString, QStringList> mPaletteGroups;
	QMap<QString, QString> mPaletteGroupsDescriptions;
	QList<ImportSpecification> mImports;
	QStringList mSubDiagramNames;

	bool initGraphicTypes(QDomElement const &graphicTypesElement);
	bool initNonGraphicTypes(QDomElement const &nonGraphicTypesElement);
	void initPaletteGroups(QDomElement const &paletteGroupsElement);
	void initSubDiagramNames(QDomElement const &subDiagramNamesElement);
};
