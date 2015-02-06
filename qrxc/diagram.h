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
	Diagram(const QString &name, const QString &nodeName, const QString &displayedName, Editor *editor);
	~Diagram();
	bool init(QDomElement const &diagramElement);
	bool resolve();
	Editor *editor() const;
	Type *findType(QString name);
	QMap<QString, Type*> types() const;
	QString name() const;
	QString nodeName() const;
	QString displayedName() const;
	QString getGroupsXML() const;
	QList<QPair<QString, QStringList>> paletteGroups() const;
	QMap<QString, QString> paletteGroupsDescriptions() const;
	bool shallPaletteBeSorted() const;

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
	QList<QPair<QString, QStringList>> mPaletteGroups;
	QMap<QString, QString> mPaletteGroupsDescriptions;
	QList<ImportSpecification> mImports;
	bool mShallPaletteBeSorted;

	bool initGraphicTypes(QDomElement const &graphicTypesElement);
	bool initNonGraphicTypes(QDomElement const &nonGraphicTypesElement);
	void initPalette(QDomElement const &paletteElement);
};
