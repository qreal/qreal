#pragma once

#include <QtXml/QDomDocument>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <QtCore/QPair>
#include <QtCore/QSet>

class XmlCompiler;
class Diagram;
class Type;
class EnumType;

namespace utils {
	class OutFile;
}

class Editor
{
public:
	Editor(QDomDocument domDocument, XmlCompiler *xmlCompiler);
	~Editor();
	XmlCompiler *xmlCompiler();
	bool isLoaded();
	bool load(QDir const &currentDir);
	Type *findType(QString const &name);
	QSet<EnumType*> getAllEnumTypes();
	Diagram *findDiagram(QString const &name);
	QMap<QString, Diagram*> diagrams();

	void generateListenerIncludes(utils::OutFile &out) const;
	void generateListenerFactory(utils::OutFile &out, QString const &pluginName) const;

private:
	XmlCompiler *mXmlCompiler;
	QDomDocument mXmlDomDocument;
	bool mLoadingComplete;
	QList<Editor*> mIncludes;
	QMap<QString, Diagram*> mDiagrams;
	QList<QPair<QString, QString> > mListeners;
};
