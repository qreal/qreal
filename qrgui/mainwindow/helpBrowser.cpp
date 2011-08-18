#include "helpBrowser.h"

HelpBrowser::HelpBrowser(QHelpEngine *engine, QWidget *parent) : QTextBrowser(parent), mHelpEngine(engine)
{
}

HelpBrowser::~HelpBrowser()
{
	delete mHelpEngine;
}

QVariant HelpBrowser::loadResource(int type, const QUrl &url)
{
	if (url.scheme() == "qthelp")
		return QVariant(mHelpEngine->fileData(url));
	else
		return QTextBrowser::loadResource(type, url);
}

