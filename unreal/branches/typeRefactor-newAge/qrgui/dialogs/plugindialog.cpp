#include <QPluginLoader>
#include <QStringList>
#include <QDir>

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>

#include <QtDebug>

#include "plugindialog.h"

#include "../pluginInterface/editorInterface.h"
#include "../editorManager/editorManager.h"

using namespace qReal;

PluginDialog::PluginDialog(const EditorManager &mgr,
		QWidget *parent) :
	QDialog(parent),
	label(new QLabel),
	treeWidget(new QTreeWidget),
	okButton(new QPushButton(tr("OK")))
{
	treeWidget->setAlternatingRowColors(false);
	treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
	treeWidget->setColumnCount(1);
	treeWidget->header()->hide();

	okButton->setDefault(true);

	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(2, 1);
	mainLayout->addWidget(label, 0, 0, 1, 3);
	mainLayout->addWidget(treeWidget, 1, 0, 1, 3);
	mainLayout->addWidget(okButton, 2, 1);
	setLayout(mainLayout);

	interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
			QIcon::Normal, QIcon::On);
	interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
			QIcon::Normal, QIcon::Off);
	featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

	setWindowTitle(tr("Plugin Information"));

        foreach (NewType editor, mgr.editors()) {
		QTreeWidgetItem *pluginItem = new QTreeWidgetItem(treeWidget);
		pluginItem->setText(0, mgr.friendlyName(editor));
		treeWidget->setItemExpanded(pluginItem, true);

		QFont boldFont = pluginItem->font(0);
		boldFont.setBold(true);
		pluginItem->setFont(0, boldFont);

                foreach (NewType diagram, mgr.diagrams(editor)) {
			QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
			interfaceItem->setText(0, mgr.friendlyName(diagram));
			interfaceItem->setIcon(0, interfaceIcon);

                        foreach (NewType element, mgr.elements(diagram)) {
				QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
				featureItem->setText(0, mgr.friendlyName(element));
				featureItem->setIcon(0, featureIcon);
			}
		}
	}
}
