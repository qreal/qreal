#pragma once

#include <QtCore/QString>

class ExternBrowser
{
public:
	ExternBrowser();
	bool openPage(QString const &url);
};
