#include "codeManager.h"

using namespace qReal;

CodeManager::CodeManager(SystemEvents *systemEvents)
	: mCurrentCodeNumber(0)
	, mSystemEvents(systemEvents)
	, mCodePath(new QMap<CodeArea *, QFileInfo>())
	, mDiagramCode(new QMap<Id, CodeArea *>())
	, mCodeDiagram(new QMap<CodeArea *, Id>())
{
	connect(mSystemEvents, SIGNAL(codePathChanged(CodeArea*, QFileInfo)), this, SLOT(changeCodePath(CodeArea*, QFileInfo)));
}

void CodeManager::addNewCode(Id diagram, CodeArea *codeArea, QFileInfo const &fileInfo)
{
	mDiagramCode->insert(diagram, codeArea);
	mCodeDiagram->insert(codeArea, diagram);
	mCodePath->insert(codeArea, fileInfo);
}

void CodeManager::nextCodeNumber()
{
	mCurrentCodeNumber++;
}

int CodeManager::currentCodeNumber()
{
	return mCurrentCodeNumber;
}

QFileInfo CodeManager::codePath(Id diagram)
{
	return mCodePath->value(mDiagramCode->value(diagram));
}

Id CodeManager::diagram(CodeArea *codeArea)
{
	return mCodeDiagram->value(codeArea);
}

void CodeManager::changeCodePath(CodeArea *codeArea, QFileInfo const &fileInfo)
{
	mCodePath->insert(codeArea, fileInfo);
}
