#include <QDebug>
#include <QTextEdit>
#include <QProcess>

#include "helpBrowser.h"
#include "externBrowser.h"

HelpBrowser::HelpBrowser(QHelpEngine *engine, QWidget *parent) : QTextBrowser(parent), mHelpEngine(engine)
{
}

HelpBrowser::~HelpBrowser()
{
	delete mHelpEngine;
}

QVariant HelpBrowser::loadResource(int type, const QUrl &url)
{
	if (url.scheme() == "qthelp") {
		return QVariant(mHelpEngine->fileData(url));
	} else {
		ExternBrowser br;
		br.openPage(url.toString());
		return 0;
	}
}

