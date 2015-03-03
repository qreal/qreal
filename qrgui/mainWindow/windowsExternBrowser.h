#pragma once

#include <QtCore/QString>

class ExternBrowser
{
public:
	ExternBrowser();
	bool openPage(const QString &url);
};
