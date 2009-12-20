#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QPair>

typedef QPair<QObject*, QString> Test;

class TestSuiteInterface
{
public:
	virtual ~TestSuiteInterface() {}

	virtual QString suiteName() = 0;
	virtual QList<Test> tests() = 0;
};


Q_DECLARE_INTERFACE(TestSuiteInterface, "ru.tepkom.QReal.TestSuiteInterface/1")
