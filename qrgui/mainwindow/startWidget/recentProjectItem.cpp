#include "recentProjectItem.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QCommandLinkButton>

#include <qrutils/textElider.h>
#include "brandManager/brandManager.h"

using namespace qReal;
using namespace utils;

RecentProjectItem::RecentProjectItem(QWidget *parent, QString const &projectName, QString const &projectFullName)
	: QPushButton(parent)
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	setFlat(true);
	setFocusPolicy(Qt::NoFocus);
	setStyleSheet(BrandManager::styles()->recentProjectButtonStyle());

	QString const nameText = QString("•  %1").arg(projectName);
	QLabel * const name = new QLabel(nameText);
	name->setStyleSheet(BrandManager::styles()->headerLevel3Style());
	name->setWordWrap(true);
	name->setAttribute(Qt::WA_TransparentForMouseEvents);

	QLabel * const fullName = new QLabel(projectFullName);
	fullName->setStyleSheet(BrandManager::styles()->headerLevel4Style());
	fullName->setWordWrap(true);
	fullName->setAttribute(Qt::WA_TransparentForMouseEvents);

	QVBoxLayout * const resultingLayout = new QVBoxLayout;

	resultingLayout->addWidget(name);
	resultingLayout->addWidget(fullName);
	setLayout(resultingLayout);

	QSignalMapper * const mapper = new QSignalMapper(this);
	mapper->setMapping(this, projectFullName);
	connect(this, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(mapper, SIGNAL(mapped(QString)), parent, SLOT(openRecentProject(QString)));
}
