#include "datatypesChecker.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace databasesSupport;

DatatypesChecker::DatatypesChecker(QString workDir, qReal::ErrorReporterInterface *errorReporter)
	: mDatatypesGlossaryFileName("../plugins/databases/databasesSupport/generator/glossary/datatypesRegExp.txt")
	, mWorkDir(workDir)
	, mErrorReporter(errorReporter)
{
	QFile datatypesGlossary(mDatatypesGlossaryFileName);
	QString line;
	if (datatypesGlossary.open(QIODevice::ReadOnly)) {
		QTextStream textStream(&datatypesGlossary);
			while (!textStream.atEnd()) {
				line = textStream.readLine();
				line.trimmed();
				if ((line != "") && (line != "\n")&& (line != "	" ))
					mDatatypesRegExps.append(QRegExp(line));
			}
		datatypesGlossary.close();
	}
}
bool DatatypesChecker::isDatatype(QString str)
{
	foreach (QRegExp regExp, mDatatypesRegExps) {
		bool isMatch = regExp.exactMatch(str);
		if (isMatch)
			return true;
	}
	return false;
}

DatatypesChecker::~DatatypesChecker()
{

}
