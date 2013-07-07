#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QFileInfo>

#include "../../qrgui/toolPluginInterface/systemEvents.h"
#include "codeManagerInterface.h"

namespace qReal {

class CodeManager : public CodeManagerInterface
{

public:
	explicit CodeManager(SystemEvents *systemEvents);

	void addNewCode(Id diagram, CodeArea *codeArea, QFileInfo const &fileInfo);
	void nextCodeNumber();
	int currentCodeNumber();
	QFileInfo codePath(Id diagram);
	Id diagram(CodeArea *codeArea);

private slots:
	void changeCodePath(CodeArea *codeArea, QFileInfo const &fileInfo);

private:
	int mCurrentCodeNumber;
	SystemEvents *mSystemEvents;
	QMap<CodeArea *, QFileInfo> *mCodePath;
	QMap<Id, CodeArea *> *mDiagramCode;
	QMap<CodeArea *, Id> *mCodeDiagram;
};

}

