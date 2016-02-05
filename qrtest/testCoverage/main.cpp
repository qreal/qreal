/* Copyright 2015 QReal Research Group
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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QVariant>

#include "dirtree.h"

namespace testCoverage {

static const int SPECTRUM = 7;

enum class Bgcolors {
	yellow,
	white,
	pink,
	orange,
	light1,
	light2,
	light3
};

// NOTE: strongly relative to Bgcolors!
QString colors[SPECTRUM] = {"yellow", "white", "pink", "orange", "00CC66", "CC99CC", "00CCCC"};

namespace dataCount {

unsigned int totalTestingFunCount = 0;
unsigned int totalVirtualCount = 0;
unsigned int totalDocumentedCount = 0;

QStringList ignoreFiles;
QString outputStats = "";
QStringList testpathes;
DirTree *dirTree = nullptr;

// .f.f absolutePath, .f.s.f name, .f.s.s.f testCount, .f.s.s.s wasUsed, .s unallocated
QList<QPair<QPair<QString, QPair<QString, QPair<int, bool> > > , bool > > listOfTests;

QString colorToString(Bgcolors color)
{
	return colors[static_cast<int>(color)];
}

Bgcolors nextColor(Bgcolors color)
{
	return static_cast<Bgcolors>((static_cast<int>(color) + 1) % SPECTRUM);
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
	const int publics = str.count(QString("public:"));
	const int index = str.indexOf(QString("public:"));
	str.remove(0, index + 2);
	if (publics > 1) {
		QRegExp reg("class.*public:");
		reg.setMinimal(true);
		str.remove(reg);
	}

	const int count = str.count("{");
	for (int i = 1; i <= count; ++i) {
		str.remove(QRegExp("\\{[^\\{\\}]*\\}"));
	}

	return str;
}

unsigned int virtualMethodsCount(const QString &str)
{
	return str.count(QRegExp("\\bvirtual\\b"));
}

unsigned int macrosesWithParameters(const QString &str)
{
	return str.count(QRegExp("Q_[A-Z]+(\\s)*\\([^\\)]*\\)"));
}

bool isIgnored(const QString &fileName)
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

unsigned int localFunctionCount(const QString &path, const QString &fileName)
{
	QFile file(path + fileName + ".h");
	// the second condition need for events when .h is interface
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text) || !QFile::exists(path + fileName + ".cpp")
			|| isIgnored(path + fileName + ".h"))
	{
		return 0;
	}

	QString header = file.readAll();
	header = commentFreeString(header);
	header = removeImplementations(header);

	QRegExp functions("[^A-Za-z0-9~_]~?[_A-Za-z0-9]+(\\s)*\\([^\\)]*\\)");
	QRegExp operators("[^A-Za-z0-9~_]operator[>!=<\\+\\*-\\|\\^&/]+(\\s)*\\([^\\)]*\\)");
	unsigned int virtCount = virtualMethodsCount(header);
	unsigned int localCount = header.count(functions)
			+ header.count(operators) - virtCount - macrosesWithParameters(header);
	totalVirtualCount += virtCount;
	return localCount;
}

int localTests(const QString &fileName, const QString &dir)
{
	int testCount = 0;
	QString path;
	// there are opportunity to binsearch
	for (int i = 0; i < listOfTests.length(); ++i) {
		if (listOfTests.at(i).second) { // if test suit has already been unallocated
			continue;
		}

		QString nameTestFile = listOfTests.at(i).first.second.first;
		QStringList dirFolders = dir.split("/", QString::SkipEmptyParts);
		// NOTE: used the most easy and dangerous way to search an appropriative list of tests (it can be clarified)
		path = listOfTests.at(i).first.first;
		path.chop(path.length() - path.lastIndexOf("/"));
		if ((nameTestFile.compare("/" + fileName + ".h", Qt::CaseInsensitive) == 0)
				&& path.contains(dirFolders.last(), Qt::CaseInsensitive)
				&& path.contains(dirFolders.at(dirFolders.length() - 2), Qt::CaseInsensitive))
		{
			testCount = listOfTests.at(i).first.second.second.first;
			if (listOfTests.at(i).first.second.second.second) {
				// test suit has already been unallocated
				listOfTests.removeAt(i);
				listOfTests.insert(i, qMakePair(qMakePair(path
						, qMakePair(nameTestFile, qMakePair(testCount, false))), true));
				return 0;
			}

			listOfTests.removeAt(i);
			listOfTests.insert(i, qMakePair(qMakePair(path
					, qMakePair(nameTestFile, qMakePair(testCount, true))), false));
			return testCount;
		}
	}

	return 0;
}

void dirTreeInitialization(const QString &name, int localTesting
		, int localDocumented, int localTests = 0, bool isIgnored = false, Bgcolors color = Bgcolors::white)
{
	dirTree = new DirTree(name, localTesting, localDocumented, localTests, isIgnored, color);
}

// WARNING: truncate correctly works if "qreal" is a sole folder with name "qreal" among subfolders
QString truncate(QString path)
{
	const int mainDirPos = path.lastIndexOf("qreal/");
	QString purePath = path.right(path.length() - mainDirPos);
	return purePath;
}

// without interface methods
void totalFunctionCount(const QString &dir, bool dirIsIgnored, DirNode *parent)
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
			localTestCount += localTests(file, truncate(dir));
		}

		totalTestingFunCount += localCount;
		if (!dirTree) {
			dirTreeInitialization(dir, localCount, totalDocumentedCount - oldDocumentedCount
					, localTestCount, false, Bgcolors::orange);
			node = dirTree->getRoot();
		} else {
			node = DirTree::createNode(dir, localCount, totalDocumentedCount - oldDocumentedCount
					, localTestCount, false, nextColor(parent->color));
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
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		list.append(line);
	}

	file.close();
}

void fillOutputStats(DirNode *node)
{
	QString name1 = "<tr bgcolor =" + colorToString(node->color) + "><td>";
	if (node->childNode) {
		name1.append("<a href=\"#" + truncate(node->childNode->name) + "\">");
	}

	name1.append((node->isIgnored ? "<i>~" : "") + truncate(node->name)
			+ (node->isIgnored ? "</i>" : "") + "</a></td>");
	QString name2 = "<td> <a name=\"" + truncate(node->name)
			+ "\"></a>" + QString::number(node->localTesting) + "</td>";
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

void fillLog(const QString &fileName)
{
	QString frame1 = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" "
			+ QString("\"http://www.w3.org/TR/html4/strict.dtd\">")
			+ QString("<html><head><meta http-equiv=Content-Type content=text/html; charset=utf-8><title>method's ")
			+ fileName + "</title><caption>" + "<font size=\"5\" face=\"Arial\"><div align=\"center\">"
			+ "Project method's information</div></font>"
			+ "<div align=\"right\"><a href=\"#Information\">Information about unallocated tests</a></div>"
			+ "</caption></head><body><table border=1 "
			+ "width=100% cellpadding=5 cols=7 bgcolor=white><tr bgcolor = pink><th width=40%>path</th>"
			+ "<th width=10%>testing</th><th width=10%>doc</th><th width=10%>tests"
			+ "</th><th width=10%>testing in subfolders</th><th width=10%>doc in subfolders"
			+ "</th><th width=10%>tests in subfolders</th width=10%></tr>";
	QString frame2 = "</table></body></html>";
	outputStats.append(frame1);
	fillNodeLogHTML(dirTree->getRoot());
	outputStats.append(frame2);
}

void localCountOfTests(QString cppFile)
{
	QFile file(cppFile);
	QString path = cppFile;
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
	listOfTests.append(qMakePair(qMakePair(truncate(path), qMakePair(hFile, qMakePair(testCount, false))), false));
}

void fillListOfTests(const QString &dir, const QString &subDir)
{
	QString absoluteDir = dir + subDir + "/";
	QDir directory(absoluteDir);
	QString fileFilter = "*.cpp";
	QStringList dirList = directory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	QStringList fileList = directory.entryList(QDir::Files);
	QStringList filterFileList = fileList.filter(QRegExp(fileFilter, Qt::CaseInsensitive, QRegExp::Wildcard));

	for (int i = 0; i < filterFileList.length(); ++i) {
		QString file = filterFileList.at(i);
		localCountOfTests(absoluteDir + file);
	}

	for (int i = 0; i < dirList.length(); ++i) {
		QString subDirNew = dirList.at(i);
		fillListOfTests(absoluteDir, subDirNew);
	}
}

void fillListOfTestDirects(const QString &dir)
{
	QDir directory(dir);
	QString pathFilter = "*Tests"; // the assumption about pathFilter
	QStringList dirList = directory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	QStringList dirFilterList = dirList.filter(QRegExp(pathFilter, Qt::CaseSensitive, QRegExp::Wildcard));

	for (int i = 0; i < dirFilterList.length(); ++i) {
		QString subDir = dirFilterList.at(i);
		if (subDir.compare("exampleTests") != 0 && subDir.compare("guiTests") != 0) {
			fillListOfTests(dir, subDir);
		}
	}
}

}
}

using namespace testCoverage;
using namespace testCoverage::dataCount;

int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	readFromFileToList("~testignore", ignoreFiles);
	readFromFileToList("testpathes", testpathes);
	for (int j = 0; j < testpathes.length(); ++j) {
		// the assumption about "tests" folder location
		fillListOfTestDirects(testpathes.at(j) + "qrtest/unitTests/");
		totalFunctionCount(testpathes.at(j), false, nullptr);
		dirTree->calculateTotalData();
		QString fileName = "testCoverage";
		QString num = (j) ? QString::number(j) : "";
		QString extension = ".html";
		QFile outputFile(fileName + num + extension);
		if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			return 0;
		}

		fillLog(fileName);
		QTextStream out(&outputFile);
		out << outputStats << "<br><font color=\"#CC0000\"><a name=Information>"
				<< "Information about unallocated tests:</a></font><br>";
		for (int i = 0; i < listOfTests.length(); ++i) {
			if (!listOfTests.at(i).first.second.second.second) {
				out << "<br>unallocated tests: <b>" << listOfTests.at(i).first.second.second.first
						<< "</b> in <font color=\"#336699\">" << listOfTests.at(i).first.first << "</font>";
			}
		}

		totalTestingFunCount = 0;
		totalVirtualCount = 0;
		totalDocumentedCount = 0;
		dirTree->~DirTree();
		dirTree = nullptr;
		outputStats.clear();
		outputFile.close();
		listOfTests.clear();
	}

	return 0;
}
