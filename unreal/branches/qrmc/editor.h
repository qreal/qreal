#pragma once

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <QtCore/QPair>
#include <QtCore/QSet>

#include "../../trunk/qrrepo/repoApi.h"


class MetaCompiler;
class Diagram;
class Type;
class EnumType;

namespace utils {
	class OutFile;
}

class Editor
{
public:
	Editor(MetaCompiler *metaCompiler, qrRepo::RepoApi *api, qReal::Id const &id);
	~Editor();
	MetaCompiler *metaCompiler();
	bool isLoaded();
	bool load();
	Type *findType(QString const &name);
	QSet<EnumType*> getAllEnumTypes();
	Diagram *findDiagram(QString const &name);
	QMap<QString, Diagram*> diagrams();

private:
	MetaCompiler *mMetaCompiler;
	qrRepo::RepoApi *mApi;
	qReal::Id mId;
	bool mLoadingComplete;
	QList<Editor*> mIncludes;
	QMap<QString, Diagram*> mDiagrams;
};
