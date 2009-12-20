#pragma once

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtGui/QIcon>

#include "../testRunner/testSuiteInterface.h"

class IdTestSuite : public QObject, public TestSuiteInterface
{
	Q_OBJECT
	Q_INTERFACES(TestSuiteInterface)

public:
	IdTestSuite();

	virtual QString suiteName();
	virtual QList<Test> tests();

private:
};

