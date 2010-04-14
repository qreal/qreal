#pragma once

#include <QDomDocument>
#include <QList>
#include <QMap>
#include <QDir>

class XmlCompiler;
class Diagram;
class Type;

class Editor
{
public:
	Editor(QDomDocument domDocument, XmlCompiler *xmlCompiler);
	~Editor();
	XmlCompiler *xmlCompiler();
	bool isLoaded();
	bool load(QDir const &currentDir);
	Type *findType(QString const &name);
	Diagram *findDiagram(QString const &name);
	QMap<QString, Diagram*> diagrams();

private:
	XmlCompiler *mXmlCompiler;
	QDomDocument mXmlDomDocument;
	bool mLoadingComplete;
	QList<Editor*> mIncludes;
	QMap<QString, Diagram*> mDiagrams;
};
