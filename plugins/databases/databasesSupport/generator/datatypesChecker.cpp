#include "datatypesChecker.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace databasesSupport;

DatatypesChecker::DatatypesChecker(QString workDir)
	: mDatatypesGlossaryFileName("../plugins/databases/databasesSupport/generator/glossary/datatypes.txt")
	, mWorkDir(workDir)
{
	bool d = isTwoParameters("(3,)");
	bool s = isTwoParameters(" (3 ,24)");
	bool r = isTwoParameters("(3,34 ) ");

	QFile datatypesGlossary(mDatatypesGlossaryFileName);
	QString line;
	if (datatypesGlossary.open(QIODevice::ReadOnly)) {
		QTextStream textStream(&datatypesGlossary);
			while (!textStream.atEnd()) {
				line = textStream.readLine();
				line.trimmed();
				if ((line != "") && (line != "\n")&& (line != "	" ))
					mDatatypes << line;
			}
		datatypesGlossary.close();
	}
}

bool DatatypesChecker::isOneParameter(const QString &str)
{
	QRegExp regExp("\\s?\\(\\s?[0-9]+\\s?\\)\\s?");
	return regExp.exactMatch(str);
}

bool DatatypesChecker::isTwoParameters(const QString &str)
{
	QRegExp regExp("\\s?\\(\\s?[0-9]+\\s?\\,\\s?[0-9]+\\s?\\)\\s?");
	return regExp.exactMatch(str);
}

bool DatatypesChecker::isDatatype(QString str)
{


}

DatatypesChecker::~DatatypesChecker()
{

}
