#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>
#include <QtCore/QMetaMethod>
#include <QtXml/QDomDocument>

#include <QtTest/QTest>

#include "testSuiteInterface.h"

QString const testLog = "testlog.txt";

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	mUi(new Ui::MainWindow)
{
	mUi->setupUi(this);
	mUi->progressBar->setMinimum(0);

	loadTestSuites();
}

MainWindow::~MainWindow()
{
	delete mUi;
}

void MainWindow::loadTestSuites()
{
	mUi->treeWidget->clear();
	mTestItems.clear();
	mTestCaseItems.clear();
	mUi->progressBar->setValue(0);
	setProgressBarStyle("gray");

	QDir testSuitesDir = suitesDir();
	if (testSuitesDir == QDir())
		return;

	foreach (QString fileName, testSuitesDir.entryList(QDir::Files)) {
		QPluginLoader loader(testSuitesDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();

		if (plugin) {
			TestSuiteInterface *iTestSuite = qobject_cast<TestSuiteInterface *>(plugin);
			if (iTestSuite) {
				QTreeWidgetItem *suiteItem = new QTreeWidgetItem(mUi->treeWidget);
				suiteItem->setText(0, iTestSuite->suiteName());
				suiteItem->setTextColor(0, Qt::gray);
				mUi->treeWidget->insertTopLevelItem(mUi->treeWidget->topLevelItemCount(), suiteItem);

				foreach (Test test, iTestSuite->tests()) {
					QTreeWidgetItem *testItem = new QTreeWidgetItem(suiteItem);
					testItem->setTextColor(0, Qt::gray);
					testItem->setText(0, test.second);

					TestItem testItemRecord = { suiteItem, testItem, test.first };
					mTestItems.append(testItemRecord);
					fillTestCases(test.first, testItem);
				}
				loader.unload();
			}
		} else
			qDebug() << "Test suite loading failed: " << loader.errorString();
	}

	mUi->treeWidget->expandAll();
	mUi->progressBar->setMaximum(mTestItems.size());
}

static bool isValidSlot(const QMetaMethod &sl)
{
	if (sl.access() != QMetaMethod::Private || !sl.parameterTypes().isEmpty()
		|| qstrlen(sl.typeName()) || sl.methodType() != QMetaMethod::Slot)
		return false;
	const char *sig = sl.signature();
	int len = qstrlen(sig);
	if (len < 2)
		return false;
	if (sig[len - 2] != '(' || sig[len - 1] != ')')
		return false;
	if (len > 7 && strcmp(sig + (len - 7), "_data()") == 0)
		return false;
	if (strcmp(sig, "initTestCase()") == 0 || strcmp(sig, "cleanupTestCase()") == 0
		|| strcmp(sig, "cleanup()") == 0 || strcmp(sig, "init()") == 0)
		return false;
	return true;
}

void MainWindow::fillTestCases(QObject *test, QTreeWidgetItem *treeItem)
{
	QMetaObject const *metaObject = test->metaObject();

	int methodCount = metaObject->methodCount();
	for (int i = 0; i < methodCount; ++i) {
		QMetaMethod slotMethod = metaObject->method(i);
		if (!isValidSlot(slotMethod))
			continue;

		QTreeWidgetItem *testCaseTreeItem = new QTreeWidgetItem(treeItem);
		QString testCaseName = slotMethod.signature();
		testCaseName.chop(2);
		testCaseTreeItem->setText(0, testCaseName);
		testCaseTreeItem->setTextColor(0, Qt::gray);

		TestCaseItem testCaseItem = {"", testCaseTreeItem};
		mTestCaseItems.insert(testCaseName, testCaseItem);
	}
}

void MainWindow::setProgressBarStyle(QString const &color)
{
	mUi->progressBar->setStyleSheet("QProgressBar:horizontal {"
		"border: 1px solid gray;"
		"border-radius: 3px;"
		"background: white;"
		"padding: 1px;"
		"text-align: right;"
		"margin-right: 10ex;"
		"}"
		"QProgressBar::chunk:horizontal {"
		"background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 " + color + QString(", stop: 1 white);")
		+ QString("}")
	);
}

QDir MainWindow::suitesDir()
{
	QDir testSuitesDir = QDir(qApp->applicationDirPath());

	while (!testSuitesDir.isRoot() && !testSuitesDir.entryList(QDir::Dirs).contains("suites")) {
		testSuitesDir.cdUp();
	}

	if (!testSuitesDir.entryList(QDir::Dirs).contains("suites"))
		return QDir();

	testSuitesDir.cd("suites");
	return testSuitesDir;
}

void MainWindow::runTests()
{
	mUi->progressBar->setValue(0);
	setProgressBarStyle("gray");
	foreach (TestItem testItem, mTestItems) {
		testItem.suiteTreeItem->setTextColor(0, Qt::gray);
		testItem.testTreeItem->setTextColor(0, Qt::gray);
	}

	QDir testSuitesDir = suitesDir();
	if (testSuitesDir == QDir())
		return;

	QList<QPluginLoader*> loadedPlugins;

	foreach (QString fileName, testSuitesDir.entryList(QDir::Files)) {
		QPluginLoader *loader = new QPluginLoader(testSuitesDir.absoluteFilePath(fileName));
		QObject *plugin = loader->instance();

		if (plugin)
			loadedPlugins.append(loader);
	}

	mFailed = false;
	foreach (TestItem testItem, mTestItems) {
		QStringList params;
		params << "" << "-o" << testLog << "-xml";
		if (QTest::qExec(testItem.test, params) == 0)
		{
			testItem.testTreeItem->setTextColor(0, Qt::green);
			if (testItem.suiteTreeItem->textColor(0) != Qt::red)
				testItem.suiteTreeItem->setTextColor(0, Qt::green);
		} else {
			testItem.testTreeItem->setTextColor(0, Qt::red);
			testItem.suiteTreeItem->setTextColor(0, Qt::red);
			mFailed = true;
		}
		analyzeLog();
		mUi->progressBar->setValue(mUi->progressBar->value() + 1);
	}

	foreach (QPluginLoader* loader, loadedPlugins) {
		loader->unload();
		loadedPlugins.removeAll(loader);
		delete loader;
	}
}

void MainWindow::analyzeLog()
{
	QFile file(testLog);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "cannot open file" << testLog;
		return;
	}

	QDomDocument log;
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;

	if (!log.setContent(&file, false, &error, &errorLine, &errorCol))
	{
		qDebug() << "parse error in " << testLog
				<< " at (" << errorLine << "," << errorCol
				<< "): " << error;
		file.close();
		return;
	}
	file.close();

	QDomNodeList results = log.elementsByTagName("TestFunction");
	for (int i = 0; i < results.count(); ++i) {
		QDomNode result = results.at(i);
		QDomElement element = result.toElement();
		QString testName = element.attribute("name", "");
		if (!mTestCaseItems.contains(testName))
			continue;
		QDomElement incident = element.childNodes().at(0).toElement();
		QString resultString = incident.attribute("type");

		TestCaseItem &item = mTestCaseItems[testName];
		if (resultString == "pass") {
			item.errorMessage = "pass";
			item.treeItem->setTextColor(0, Qt::green);
		} else if (resultString == "fail") {
			QDomElement description = element.childNodes().at(0).toElement();
			QString descriptionString = description.text();

			QString fileString = incident.attribute("file");
			QString lineString = incident.attribute("line");

			item.errorMessage = fileString + ":" + lineString + " - " + descriptionString;
			item.treeItem->setTextColor(0, Qt::red);
		} else {
			item.errorMessage = "Unknown error";
			item.treeItem->setTextColor(0, Qt::red);
		}
	}
}

void MainWindow::progressBarValueChanged(int value)
{
	if (mUi->progressBar->value() == mUi->progressBar->maximum()) {
		if (mFailed)
			setProgressBarStyle("red");
		else
			setProgressBarStyle("green");
	}
}

void MainWindow::treeSelectedItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if (current == NULL) {
		mUi->textBrowser->setText("");
		return;
	}

	QString testName = current->text(0);
	qDebug() << testName << "ololo";
	if (mTestCaseItems.contains(testName)) {
		qDebug() << "ololo!";
		mUi->textBrowser->setText(mTestCaseItems[testName].errorMessage);
	} else
		mUi->textBrowser->setText("");
}

void MainWindow::reloadSuitesClicked()
{
	loadTestSuites();
}

void MainWindow::runTestsClicked()
{
	runTests();
}
