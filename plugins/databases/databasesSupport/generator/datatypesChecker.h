#pragma once

#include <QtCore/QTranslator>
#include <qrgui/mainwindow/errorReporter.h>
#include <qrkernel/ids.h>

namespace qReal {
namespace databasesSupport {

class DatatypesChecker: public QObject
{
	Q_OBJECT
public:
	DatatypesChecker(const QString workDir, qReal::ErrorReporterInterface *errorReporter);
	~DatatypesChecker();

	bool isDatatype(QString str);
private:
	QString mDatatypesGlossaryFileName;
	QString mWorkDir;
	QList<QRegExp> mDatatypesRegExps;
	qReal::ErrorReporterInterface *mErrorReporter;
};

}
}
