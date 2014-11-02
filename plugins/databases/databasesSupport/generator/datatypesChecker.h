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
	DatatypesChecker(const QString workDir);
	~DatatypesChecker();

	bool isDatatype(QString str);
private:
	bool isOneParameter(const QString &str);
	bool isTwoParameters(const QString &str);
	QString mDatatypesGlossaryFileName;
	QString mWorkDir;
	QStringList mDatatypes;
};

}
}
