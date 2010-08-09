#pragma once
#include "fixer.h"
#include <QString>

class ProjectTypeFixer : Fixer
{
public:
	virtual void fix();
private:
	void fixProject(const QString &fileName);
};
