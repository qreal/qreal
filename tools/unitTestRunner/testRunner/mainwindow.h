#pragma once

#include <QMainWindow>
#include <QtCore/QDir>
#include <QtCore/QHash>
#include <QtCore/QString>

class QTreeWidgetItem;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void reloadSuitesClicked();
	void runTestsClicked();

	void progressBarValueChanged(int value);
	void treeSelectedItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
	struct TestItem {
		QTreeWidgetItem *testTreeItem;
		QTreeWidgetItem *suiteTreeItem;
		QObject *test;
	};

	struct TestCaseItem {
		QString errorMessage;
		QTreeWidgetItem *treeItem;
	};

	static QDir suitesDir();

	void setProgressBarStyle(QString const &color);
	void loadTestSuites();
	void runTests();
	void fillTestCases(QObject *test, QTreeWidgetItem *treeItem);
	void analyzeLog();

	Ui::MainWindow *mUi;
	QList<TestItem> mTestItems;
	QHash<QString, TestCaseItem> mTestCaseItems;
	bool mFailed;
};
