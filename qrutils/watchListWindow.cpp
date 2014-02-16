#include "watchListWindow.h"
#include "ui_watchListWindow.h"

using namespace utils;

WatchListWindow::WatchListWindow(const utils::ExpressionsParser *parser, QWidget *parent)
	: QDockWidget(parent)
	, mUi(new Ui::watchListWindow)
	, mParser(parser)
{
	mUi->setupUi(this);

	updateVariables();
	mTimer = new QTimer();
	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateVariables()));
	mTimer->start(watchWindowRefreshInterval);
}

WatchListWindow::~WatchListWindow()
{
	delete mUi;
	mTimer->stop();
	delete mTimer;
}

void WatchListWindow::updateVariables()
{
	for (int i = mUi->watchListTableWidget->rowCount() - 1; i >= 0; --i) {
		mUi->watchListTableWidget->removeRow(i);
	}

	int row = 0;
	QMap<QString, Number *> const &variables = mParser->variables();

	foreach (QString const &variable, variables.keys()) {
		mUi->watchListTableWidget->insertRow(row);
		QTableWidgetItem* item = new QTableWidgetItem(variable);
		mUi->watchListTableWidget->setItem(row, 0, item);
		item = new QTableWidgetItem(variables.value(variable)->toString());
		mUi->watchListTableWidget->setItem(row, 1, item);
		++row;
	}
}
