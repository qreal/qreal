#include <QTextEdit>
#include <QProcess>

#include "linuxExternBrowser.h"
#include "../../../../qrutils/inFile.h"

void ExternBrowser::openPage(QString const &url)
{
	//TODO
	/// Linux extern browser in progress..

	QProcess *proc = new QProcess();
	QString cmd = "xdg-open";
	//proc->start(curDirect + "/xdgOpen.sh " + url);

}
