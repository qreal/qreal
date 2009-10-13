#pragma once
#include "Fixer.h"
#include <QString>

class ProjectTypeFixer : Fixer
{
public:
	virtual void fix();
private:
	void fixProject(const QString &fileName);
};
