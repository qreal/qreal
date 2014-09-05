#include "watchListWindow.h"
#include "ui_watchListWindow.h"

using namespace utils;

WatchListWindow::WatchListWindow(qrtext::lua::LuaToolbox &newParser, QWidget *parent)
	: QDockWidget(parent)
	, mUi(new Ui::watchListWindow)
	, mNewParser(newParser)
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
	QStringList sortedIdentifiers = mNewParser.identifiers();
	qSort(sortedIdentifiers);
	for (QString const &identifier : sortedIdentifiers) {
		if (mHiddenVariables.contains(identifier)) {
			continue;
		}

		mUi->watchListTableWidget->insertRow(row);
		QTableWidgetItem* item = new QTableWidgetItem(identifier);
		mUi->watchListTableWidget->setItem(row, 0, item);
		item = new QTableWidgetItem(mNewParser.value<QString>(identifier));
		mUi->watchListTableWidget->setItem(row, 1, item);
		++row;
	}
}

void WatchListWindow::hideVariables(QStringList const &variableNames)
{
	for (QString const &variableName : variableNames) {
		mHiddenVariables.insert(variableName);
	}
}
