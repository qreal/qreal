#pragma once

#include <QtCore/QObject>
#include <QtCore/QFileInfo>

#include "codeArea.h"
#include "../../qrkernel/ids.h"

namespace qReal {

class CodeManagerInterface : public QObject
{
	Q_OBJECT

public:
	virtual void addNewCode(Id diagram, CodeArea *codeArea, QFileInfo const &fileInfo) = 0;
	virtual void nextCodeNumber() = 0;
	virtual int currentCodeNumber() = 0;
	virtual QFileInfo codePath(Id diagram) = 0;
	virtual Id diagram(CodeArea *codeArea) = 0;

private slots:
	virtual void changeCodePath(CodeArea *codeArea, QFileInfo const &fileInfo) = 0;
};

}
