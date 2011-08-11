#pragma once

#include <QtHelp/QHelpEngine>
#include <QtHelp/QHelpContentWidget>
#include <QtHelp/QHelpIndexModel>
#include <QtGui/QTextBrowser>

class HelpBrowser : public QTextBrowser
{
	Q_OBJECT;

public:
	HelpBrowser(QHelpEngine *engine, QWidget *parent = 0);
	~HelpBrowser();

	QVariant loadResource(int type, const QUrl &url);
private:
	QHelpEngine *mHelpEngine;
};
