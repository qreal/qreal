#include <QDir>
#include <QFile>
#include <stdio.h>
#include <QRegExp>
#include <QtDebug>
#include <QTime>
#include <QList>
#include <QPair>

#include "dirtree.h"

unsigned int totalTestingFunCount = 0;
unsigned int totalVirtualCount = 0;
unsigned int totalDocumentedCount = 0;

QStringList ignoreFiles;
QString outputStats = "";
QStringList testpathes;
DirTree *dirTree = nullptr;
QList<QPair<QString, QPair<int, bool> > > listOfTests;
QString warning = "";
QString colors[] = {"yellow", "white", "pink", "orange", "00CC66", "CC99CC", "00CCCC"};

QString colorToString(bgcolors color)
{
	return colors[color];
}

bgcolors nextColor(bgcolors color)
{
	return (bgcolors)(((int)color + 1) % 7);
}

QString commentFreeString(QString str)
{
	// this counting is used and accurate because we have style guide
	int docCount = str.count(QRegExp("///[^\\n]*\\n[^\\n/\\(]*\\("));
	docCount += str.count(QString("/**"));
	docCount += str.count(QRegExp("//![^\\n]*\\n[^/\\(]*\\("));
	totalDocumentedCount += docCount;
	QRegExp reg = QRegExp("/\\*.*\\*/");
	reg.setMinimal(true);
	str.remove(reg);
	str.remove(QRegExp("//[^\\n$]*(\\n|$)"));
	return str;
}

QString removeImplementations(QString str)
{
	int publics = str.count(QString("public:"));
	int index = str.indexOf(QString("public:"));
	str.remove(0, index + 2);
	if (publics > 1) {
		QRegExp reg("class.*public:");
		reg.setMinimal(true);
		str.remove(reg);
	}
	int count = str.count("{");
	for (int i = 1; i <= count; ++i) {
		str.remove(QRegExp("\\{[^\\{\\}]*\\}"));
	}

	return str;
}

unsigned int virtualMethodsCount(QString str)
{
	return str.count(QRegExp("\\bvirtual\\b"));
}

unsigned int macrosesWithParameters(QString str)
{
	return str.count(QRegExp("Q_[A-Z]+(\\s)*\\([^\\)]*\\)"));
}

bool isIgnored(QString fileName)
{
	bool isMatch = false;
	for (int j = 0; j < ignoreFiles.length(); ++j) {
		QRegExp exReg(".*" + ignoreFiles.at(j) + ".*");
		if (exReg.exactMatch(fileName)) {
			isMatch = true;
			break;
		}
	}

	return isMatch;
}

unsigned int localFunctionCount(QString path, QString fileName)
{
	QFile file(path + fileName + ".h");
	// the second condition need for events when .h is interface
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text) || !QFile::exists(path + fileName + ".cpp")
			|| isIgnored(path + fileName + ".h")) {
		return 0;
	}

	QString header = file.readAll();
	header = commentFreeString(header);
	header = removeImplementations(header);

	QRegExp functions("[^A-Za-z0-9~_]~?[_A-Za-z0-9]+(\\s)*\\([^\\)]*\\)");
	QRegExp operators("[^A-Za-z0-9~_]operator[>!=<\\+\\*-\\|\\^&/]+(\\s)*\\([^\\)]*\\)");
	unsigned int virtCount = virtualMethodsCount(header);
	unsigned int localCount = header.count(functions) + header.count(operators) - virtCount - macrosesWithParameters(header);

	totalVirtualCount += virtCount;
	return localCount;
}

int localTests(QString fileName)
{
	int testCount = 0;
	// there are opportunity to binsearch
	for (int i = 0; i < listOfTests.length(); ++i) {
		QString nameTestFile = listOfTests.at(i).first;
		if (nameTestFile.compare("/" + fileName + ".h", Qt::CaseInsensitive) == 0) {
			if (listOfTests.at(i).second.second) {
				warning = QString("There are the different classes with the same name in this project (they have unitTests) and (only) test\'s counting is wrong!") +
						QString(" Solution: the new requests in style guide about the test\'s formalization and refactoring counting code.\n");
			}
			testCount = listOfTests.at(i).second.first;
			listOfTests.removeAt(i);
			listOfTests.insert(i, qMakePair(nameTestFile, qMakePair(testCount, true)));
			break;
		}
	}

	return testCount;
}

void dirTreeInitialization(QString name, int localTesting, int localDocumented, int localTests = 0, bool isIgnored = false, bgcolors color = white)
{
	dirTree = new DirTree(name, localTesting, localDocumented, localTests, isIgnored, color);
}

// without interface methods
void totalFunctionCount(QString dir, bool dirIsIgnored, DirNode *parent)
{
	DirNode *node;
	QString filter = "*.h";
	QDir directory(dir);
	QStringList fileList = directory.entryList(QDir::Files);
	QStringList filterFileList = fileList.filter(QRegExp(filter, Qt::CaseInsensitive, QRegExp::Wildcard));
	QStringList dirList = directory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	unsigned int localCount = 0;
	int localTestCount = 0;
	unsigned int oldDocumentedCount = totalDocumentedCount;

	if (!parent || !dirIsIgnored) {
		for (int i = 0; i < filterFileList.length(); ++i) {
			QString file = filterFileList.at(i);
			file.chop(2);
			localCount += localFunctionCount(dir, file);
			localTestCount += localTests(file);
		}
		totalTestingFunCount += localCount;
		if (!dirTree) {
			dirTreeInitialization(dir, localCount, totalDocumentedCount - oldDocumentedCount, localTestCount, false, orange);
			node = dirTree->getRoot();
		} else {
			node = DirTree::createNode(dir, localCount, totalDocumentedCount - oldDocumentedCount, localTestCount, false, nextColor(parent->color));
			dirTree->addChild(node, parent);
		}

		for (int i = 0; i < dirList.length(); ++i) {
			bool followDirIsIgnored = false;
			for (int j = 0; j < ignoreFiles.length(); ++j) {
				QRegExp exReg(".*" + ignoreFiles.at(j) + ".*");
				if (exReg.exactMatch(dir + dirList.at(i) + "/")) {
					followDirIsIgnored = true;
					break;
				}
			}
			totalFunctionCount(dir + dirList.at(i) + "/", followDirIsIgnored, node);
		}
	} else {
		node = DirTree::createNode(dir, 0, 0, 0, true, nextColor(parent->color));
		dirTree->addChild(node, parent);
		for (int i = 0; i < dirList.length(); ++i) {
			totalFunctionCount(dir + dirList.at(i) + "/", true, node);
		}
	}
}

void readFromFileToList(const QString &relativeFileName, QStringList &list)
{
	QFile file(relativeFileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "not opened";
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		qDebug() << line << "readlined";
		list.append(line);
	}

	file.close();
}

void fillOutputStats(DirNode *node)
{
	QString name1 = "<tr bgcolor =" + colorToString(node->color) + "><td>";
	if (node->childNode) {
		name1.append("<a href=\"#" + node->childNode->name + "\">");
	}

	name1.append((node->isIgnored ? "<i>~" : "") + node->name + (node->isIgnored ? "</i>" : "") + "</a></td>");
	QString name2 = "<td> <a name=\"" + node->name + "\"></a>" + QString::number(node->localTesting) + "</td>";
	QString name3 = "<td>" + QString::number(node->localDocumented)+ "</td>";
	QString name4 = "<td>" + QString::number(node->localTests) + "</td>";
	QString name5 = "<td>" + QString::number(node->totalTesting - node->localTesting) + "</td>";
	QString name6 = "<td>" + QString::number(node->totalDocumented - node->localDocumented) + "</td>";
	QString name7 = "<td>" + QString::number(node->totalTests - node->localTests) + "</td></tr>";

	outputStats.append(name1 + name2 + name3 + name4 + name5 + name6 + name7);

}

void fillNodeLogHTML(DirNode *node)
{
	if (node == dirTree->getRoot()) {
		fillOutputStats(node);
	}

	DirNode *tmpNode = node->childNode;
	if (tmpNode) {
		if (tmpNode->rightNode) {
			fillOutputStats(tmpNode);
			while (tmpNode->rightNode != nullptr) {
				tmpNode = tmpNode->rightNode;
				fillOutputStats(tmpNode);
			}

			tmpNode = node->childNode;
			fillNodeLogHTML(tmpNode);
			while (tmpNode->rightNode != nullptr) {
				tmpNode = tmpNode->rightNode;
				fillNodeLogHTML(tmpNode);
			}
		} else {
			fillOutputStats(node->childNode);
			fillNodeLogHTML(node->childNode);
		}
	}
}

void fillLog(QString fileName)
{
	QString frame1 = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">"
			+ QString("<html><head><meta http-equiv=Content-Type content=text/html; charset=utf-8><title>method's ")
			+ fileName + "</title><caption>" + "Project method's information" + "</caption></head><body><table border=1 "
			+ "width=100% cellpadding=5 cols=7 bgcolor=white><tr bgcolor = pink><th width=40%>path</th><th width=10%>testing</th><th width=10%>doc</th><th width=10%>tests"
			+ "</th><th width=10%>testing in subfolders</th><th width=10%>doc in subfolders</th><th width=10%>tests in subfolders</th width=10%></tr>";
	QString frame2 = "</table></body></html>";
	outputStats.append(frame1);
	fillNodeLogHTML(dirTree->getRoot());
	outputStats.append(frame2);
}

void localCountOfTests(QString cppFile)
{
	QFile file(cppFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}
	QString impl = file.readAll();
	int testCount = impl.count(QRegExp("TEST(\\s)?\\("));
	testCount += impl.count(QRegExp("TEST_F(\\s)?\\("));
	cppFile.chop(8);
	cppFile.append(".h");
	int lastSlash = cppFile.lastIndexOf("/");
	QString hFile = cppFile.right(cppFile.length() - lastSlash);
	listOfTests.append(qMakePair(hFile, qMakePair(testCount, false)));
}

void fillListOfTests(QString dir)
{
	QDir directory(dir);
	QString fileFilter = "*.cpp";
	QStringList dirList = directory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	QStringList fileList = directory.entryList(QDir::Files);
	QStringList filterFileList = fileList.filter(QRegExp(fileFilter, Qt::CaseInsensitive, QRegExp::Wildcard));

	for (int i = 0; i < filterFileList.length(); ++i) {
		QString file = filterFileList.at(i);
		localCountOfTests(dir + file);
	}

	for (int i = 0; i < dirList.length(); ++i) {
		QString subDir = dirList.at(i);
		fillListOfTests(dir + subDir + "/");
	}
}

void fillListOfTestDirects(QString dir)
{
	QDir directory(dir);
	QString pathFilter = "*Tests";
	QStringList dirList = directory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	QStringList dirFilterList = dirList.filter(QRegExp(pathFilter, Qt::CaseSensitive, QRegExp::Wildcard));

	for (int i = 0; i < dirFilterList.length(); ++i) {
		QString subDir = dirFilterList.at(i);
		if (subDir.compare("exampleTests") != 0) {
			fillListOfTests(dir + subDir + "/");
		}
	}
}

int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	QTime time;
	time.start();

	readFromFileToList("~testignore", ignoreFiles);
	readFromFileToList("testpathes", testpathes);
	for (int j = 0; j < testpathes.length(); ++j)
	{
		fillListOfTestDirects(testpathes.at(j) + "buildScripts/");
		totalFunctionCount(testpathes.at(j), false, nullptr);
		dirTree->calculateTotalData();
		QString fileName = "testCoverage";
		QString num = (j) ? QString::number(j) : "";
		QString extension = ".html";
		QFile outputFile(fileName + num + extension);
		if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			qDebug() << "!!!";
			return 0;
		}
		fillLog(fileName);
		QTextStream out(&outputFile);
		out << warning << outputStats;
		totalTestingFunCount = 0;
		totalVirtualCount = 0;
		totalDocumentedCount = 0;
		dirTree->~DirTree();
		dirTree = nullptr;
		outputStats.clear();
		outputFile.close();
		listOfTests.clear();
		warning.clear();
		qDebug() << "fail zoz";
	}

	qDebug() << "time of execution: " + QString::number(time.elapsed()) + " ms";
	return 0;
}
