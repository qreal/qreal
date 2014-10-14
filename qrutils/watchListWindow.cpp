#include "watchListWindow.h"
#include "ui_watchListWindow.h"

#include <functional>

using namespace utils;

WatchListWindow::WatchListWindow(qrtext::DebuggerInterface const &interpreter, QWidget *parent)
	: WatchListWindow(nullptr, &interpreter, parent)
{
}

WatchListWindow::WatchListWindow(utils::ExpressionsParser const *parser, QWidget *parent)
	: WatchListWindow(parser, nullptr, parent)
{
}

WatchListWindow::WatchListWindow(utils::ExpressionsParser const * const parser
		, qrtext::DebuggerInterface const * const newParser
		, QWidget *parent)
	: QDockWidget(parent)
	, mUi(new Ui::watchListWindow)
	, mNewParser(newParser)
	, mParser(parser)
{
	mUi->setupUi(this);
	connect(&mTimer, &QTimer::timeout, this, &WatchListWindow::updateVariables);
	mTimer.start(watchWindowRefreshInterval);
}

WatchListWindow::~WatchListWindow()
{
	delete mUi;
	mTimer.stop();
}

void WatchListWindow::updateVariables()
{
	int row = 0;

	std::function<QStringList()> identifiers;
	std::function<QString(QString const &)> value;

	if (!mNewParser) {
		identifiers = [this] () { return mParser->variables().keys(); };
		value = [this] (QString const &name) { return mParser->variables().value(name)->toString(); };
	} else {
		identifiers = [this] () { return mNewParser->identifiers(); };
		value = [this] (QString const &name) { return mNewParser->value<QString>(name); };
	}

	QStringList sortedIdentifiers = identifiers();
	qSort(sortedIdentifiers);
	for (QString const &identifier : sortedIdentifiers) {
		if (mHiddenVariables.contains(identifier)) {
			continue;
		}

		if (row >= mUi->watchListTableWidget->rowCount()) {
			mUi->watchListTableWidget->insertRow(row);
			QTableWidgetItem* item = new QTableWidgetItem(identifier);
			mUi->watchListTableWidget->setItem(row, 0, item);
			item = new QTableWidgetItem(value(identifier));
			mUi->watchListTableWidget->setItem(row, 1, item);
		} else {
			mUi->watchListTableWidget->item(row, 0)->setText(identifier);
			mUi->watchListTableWidget->item(row, 1)->setText(value(identifier));
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
