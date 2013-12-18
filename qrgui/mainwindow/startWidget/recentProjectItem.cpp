#include "recentProjectItem.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include <qrutils/textElider.h>

using namespace qReal;
using namespace utils;

RecentProjectItem::RecentProjectItem(QWidget *parent, QString const &projectName, QString const &projectFullName)
	: QWidget(parent)
{
	QString const nameText = QString("<a href='%1'><font color='black'>•  %2</font></a>").arg(projectFullName, projectName);
	QLabel * const name = new QLabel(nameText);
	QLabel * const fullName = new QLabel(projectFullName);
	name->setWordWrap(true);
	fullName->setWordWrap(true);
	QVBoxLayout * const recentProject = new QVBoxLayout;

	recentProject->addWidget(name);
	recentProject->addWidget(fullName);
	setLayout(recentProject);

	connect(name, SIGNAL(linkActivated(QString)), parent, SLOT(openRecentProject(QString)));
}
