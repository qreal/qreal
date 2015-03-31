#include "watchListWindow.h"
#include "ui_watchListWindow.h"

#include <functional>

using namespace utils;

WatchListWindow::WatchListWindow(const qrtext::DebuggerInterface &interpreter, QWidget *parent)
	: WatchListWindow(nullptr, &interpreter, parent)
{
}

WatchListWindow::WatchListWindow(const utils::ExpressionsParser *parser, QWidget *parent)
	: WatchListWindow(parser, nullptr, parent)
{
}

WatchListWindow::WatchListWindow(const utils::ExpressionsParser * const parser
		, const qrtext::DebuggerInterface * const newParser
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
	std::function<QStringList(const QString &)> value;

	if (!mNewParser) {
		identifiers = [this] () { return mParser->variables().keys(); };
		value = [this] (const QString &name) { return mParser->variables().value(name)->toStringList(); };
	} else {
		identifiers = [this] () { return mNewParser->identifiers(); };
		value = [this] (const QString &name) { return mNewParser->value<QStringList>(name); };
	}

	QStringList sortedIdentifiers = identifiers();
	qSort(sortedIdentifiers);
	for (const QString &identifier : sortedIdentifiers) {
		if (mHiddenVariables.contains(identifier)) {
			continue;
		}

		if (row >= mUi->watchListTableWidget->rowCount()) {
			mUi->watchListTableWidget->insertRow(row);
			QTableWidgetItem* item = new QTableWidgetItem(identifier);
			mUi->watchListTableWidget->setItem(row, 0, item);
			const QStringList v = value(identifier);
			const QString text = v.size() == 1 ? v[0] : (v.isEmpty() ? "" : QString("{ %1 }").arg(v.join(',')));
			item = new QTableWidgetItem(text);
			mUi->watchListTableWidget->setItem(row, 1, item);
		} else {
			mUi->watchListTableWidget->item(row, 0)->setText(identifier);
			const QStringList v = value(identifier);
			const QString text = v.size() == 1 ? v[0] : (v.isEmpty() ? "" : QString("{ %1 }").arg(v.join(',')));
			mUi->watchListTableWidget->item(row, 1)->setText(text);
		}

		++row;
	}

	for (int i = row; i < mUi->watchListTableWidget->rowCount(); ++i) {
		mUi->watchListTableWidget->removeRow(row);
	}
}

void WatchListWindow::hideVariables(const QStringList &variableNames)
{
	for (const QString &variableName : variableNames) {
		mHiddenVariables.insert(variableName);
	}
}
