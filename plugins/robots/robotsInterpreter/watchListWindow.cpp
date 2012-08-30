#include <QtGui/QLayout>

#include "watchListWindow.h"
#include "ui_watchListWindow.h"

watchListWindow::watchListWindow(const utils::ExpressionsParser *parser, QWidget *parent) :
	QDialog(parent)
	, mUi(new Ui::watchListWindow)
	, mParser(parser)
{
	mUi->setupUi(this);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(mUi->watchListTableWidget);


	updateVariables();
	mTimer = new QTimer();
	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateVariables()));
	mTimer->start(250);
}

watchListWindow::~watchListWindow()
{
	delete mUi;
	mTimer->stop();
	delete mTimer;
}

void watchListWindow::updateVariables()
{
	for (int i = mUi->watchListTableWidget->rowCount() - 1; i >=0; i--) {
		mUi->watchListTableWidget->removeRow(i);
	}

	int row = 0;
	QMap<QString, QString> *variables = mParser->getVariablesForWatch();

	foreach (QString variable, variables->keys()) {
		mUi->watchListTableWidget->insertRow(row);
		QTableWidgetItem* item = new QTableWidgetItem(variable);
		mUi->watchListTableWidget->setItem(row, 0, item);
		item = new QTableWidgetItem(variables->value(variable));
		mUi->watchListTableWidget->setItem(row, 1, item);
		row++;
	}
}
