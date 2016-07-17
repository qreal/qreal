/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	std::function<QVariant(const QString &)> value;

	if (!mNewParser) {
		identifiers = [this] () { return mParser->variables().keys(); };
		value = [this] (const QString &name) { return mParser->variables().value(name)->value(); };
	} else {
		identifiers = [this] () { return mNewParser->identifiers(); };
		value = [this] (const QString &name) { return mNewParser->value<QVariant>(name); };
	}

	QStringList sortedIdentifiers = identifiers();
	qSort(sortedIdentifiers);
	for (const QString &identifier : sortedIdentifiers) {
		if (mHiddenVariables.contains(identifier)) {
			continue;
		}

		if (row >= mUi->watchListTableWidget->rowCount()) {
			mUi->watchListTableWidget->insertRow(row);
			mUi->watchListTableWidget->setItem(row, 0, new QTableWidgetItem());
			mUi->watchListTableWidget->setItem(row, 1, new QTableWidgetItem());
		}

		mUi->watchListTableWidget->item(row, 0)->setText(identifier);
		mUi->watchListTableWidget->item(row, 1)->setText(toString(value(identifier)));

		++row;
	}

	for (int i = row; i < mUi->watchListTableWidget->rowCount(); ++i) {
		mUi->watchListTableWidget->removeRow(row);
	}
}

QString WatchListWindow::toString(const QVariant &value) const
{
	if (value.type() == QVariant::StringList) {
		const QStringList list = value.toStringList();
		return list.isEmpty() ? "" : QString("{ %1 }").arg(list.join(", "));
	}

	if (value.type() == QVariant::List) {
		QString result = "{";
		for (const QVariant &i : value.value<QVariantList>()) {
			result += toString(i) + ", ";
		}

		if (result.size() >= 2) {
			result.chop(2);
		}

		result += "}";
		return result;
	}

	return value.toString();
}

void WatchListWindow::hideVariables(const QStringList &variableNames)
{
	for (const QString &variableName : variableNames) {
		mHiddenVariables.insert(variableName);
	}
}
