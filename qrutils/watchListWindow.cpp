#include "watchListWindow.h"
#include "ui_watchListWindow.h"

using namespace utils;

WatchListWindow::WatchListWindow(qrtext::DebuggerInterface &newParser, QWidget *parent)
	: QDockWidget(parent)
	, mUi(new Ui::watchListWindow)
	, mNewParser(newParser)
{
	mUi->setupUi(this);

	updateVariables();
	mTimer = new QTimer();
	connect(mTimer, &QTimer::timeout, this, &WatchListWindow::updateVariables);
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
	int row = 0;
	QStringList sortedIdentifiers = mNewParser.identifiers();
	qSort(sortedIdentifiers);
	for (QString const &identifier : sortedIdentifiers) {
		if (mHiddenVariables.contains(identifier)) {
			continue;
		}

		auto rowCount = mUi->watchListTableWidget->rowCount();
		qDebug() << rowCount;
		if (row >= mUi->watchListTableWidget->rowCount()) {
			mUi->watchListTableWidget->insertRow(row);
			QTableWidgetItem* item = new QTableWidgetItem(identifier);
			mUi->watchListTableWidget->setItem(row, 0, item);
			item = new QTableWidgetItem(mNewParser.value<QString>(identifier));
			mUi->watchListTableWidget->setItem(row, 1, item);
		} else {
			mUi->watchListTableWidget->item(row, 0)->setText(identifier);
			mUi->watchListTableWidget->item(row, 1)->setText(mNewParser.value<QString>(identifier));
		}

		++row;
	}

	for (int i = row; i < mUi->watchListTableWidget->rowCount(); ++i) {
		mUi->watchListTableWidget->removeRow(row);
	}
}

void WatchListWindow::hideVariables(QStringList const &variableNames)
{
	for (QString const &variableName : variableNames) {
		mHiddenVariables.insert(variableName);
	}
}
