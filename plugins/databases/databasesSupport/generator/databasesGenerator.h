#pragma once

#include <QtCore/QTranslator>
#include <qrgui/mainwindow/errorReporter.h>
#include <qrkernel/ids.h>
#include "datatypesChecker.h"

namespace qReal {
namespace databasesSupport {

/// Databases generator for generating SQL code
class DatabasesGenerator: public QObject
{
	Q_OBJECT
public:
	virtual void activeTabChanged(qReal::Id const &rootElementId) = 0;
	virtual void clearPhysicalModel() = 0;
public slots:
	virtual bool checkCorrectness() = 0;
	virtual void generatePhysicalModel() = 0;
	virtual void generateSQLCode() = 0;
};
}
}
