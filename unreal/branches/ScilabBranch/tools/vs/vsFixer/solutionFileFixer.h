#pragma once
#include "fixer.h"
#include <QStringList>

class SolutionFileFixer : Fixer
{
public:
	virtual void fix();
private:
	QStringList solutionFileStringList;
	void fixFile();
	void writeToQrxcDependencies();
	void writeQrGuiDependencies();
	QString findProjectHash(const QString &projectName);
};
