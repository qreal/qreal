#include <QtCore/QDebug>
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>

#include "testInvocationPlugin.h"
#include "../../qrutils/invocationUtils/functorOperation.h"

using namespace testInvocation;

Q_EXPORT_PLUGIN2("TestInvocationPlugin", testInvocation::TestInvocationPlugin)

void TestInvocationPlugin::init(const qReal::PluginConfigurator &configurator)
{
	mMainWindow = &(configurator.mainWindowInterpretersInterface());
}

QList<qReal::ActionInfo> TestInvocationPlugin::actions()
{
	QList<qReal::ActionInfo> actions;
	QMenu *svnMenu = new QMenu("Test Invocation");

	QAction *asyncWithoutProgresAction = svnMenu->addAction("Async without progress");
	connect(asyncWithoutProgresAction, SIGNAL(triggered()), this, SLOT(asyncWithoutProgress()));

	QAction *asyncWithProgresAction = svnMenu->addAction("Async with progress");
	connect(asyncWithProgresAction, SIGNAL(triggered()), this, SLOT(asyncWithProgress()));

	QAction *syncWithoutProgresAction = svnMenu->addAction("Sync without progress");
	connect(syncWithoutProgresAction, SIGNAL(triggered()), this, SLOT(syncWithoutProgress()));

	QAction *syncWithProgresAction = svnMenu->addAction("Sync with progress");
	connect(syncWithProgresAction, SIGNAL(triggered()), this, SLOT(syncWithProgress()));

	actions << qReal::ActionInfo(svnMenu, "tools");
	return actions;
}

void TestInvocationPlugin::asyncWithoutProgress()
{
	qDebug() << "async no progress";

	invocation::FunctorOperation<void> *operation = new invocation::FunctorOperation<void>(2000);
	connect(operation, SIGNAL(finished(invocation::InvocationState)), this, SLOT(onOpertionFinished(invocation::InvocationState)));
	operation->setInvocationTarget(this, &TestInvocationPlugin::doOperation, 1000, 5);
	mMainWindow->reportOperation(operation);
	operation->invoceAsync();
}

void TestInvocationPlugin::asyncWithProgress()
{
	qDebug() << "async with progress";

	invocation::FunctorOperation<void> *operation = new invocation::FunctorOperation<void>;
	connect(operation, SIGNAL(finished(invocation::InvocationState)), this, SLOT(onOpertionFinished(invocation::InvocationState)));
	operation->setInvocationTargetWithProgress(this, &TestInvocationPlugin::doOperation, 1000, 5);
	mMainWindow->reportOperation(operation);
	operation->invoceAsync();
}

void TestInvocationPlugin::syncWithoutProgress()
{
	qDebug() << "sync no progress";

	invocation::FunctorOperation<void> *operation = new invocation::FunctorOperation<void>;
	connect(operation, SIGNAL(finished(invocation::InvocationState)), this, SLOT(onOpertionFinished(invocation::InvocationState)));
	operation->setInvocationTarget(this, &TestInvocationPlugin::doOperation, 1000, 5);
	mMainWindow->reportOperation(operation);
	operation->invoceSync();
}

void TestInvocationPlugin::syncWithProgress()
{
	qDebug() << "sync with progress";

	invocation::FunctorOperation<void> *operation = new invocation::FunctorOperation<void>(2000);
	connect(operation, SIGNAL(finished(invocation::InvocationState)), this, SLOT(onOpertionFinished(invocation::InvocationState)));
	operation->setInvocationTargetWithProgress(this, &TestInvocationPlugin::doOperation, 1000, 5);
	mMainWindow->reportOperation(operation);
	operation->invoceSync();
}

void TestInvocationPlugin::doOperation(invocation::Progress *progress, int sleepInterval, int count)
{
	progress->setMinimum(0);
	progress->setMaximum(count-1);
	for (int i = 0; i < count; ++i) {
		QWaitCondition sleep;
		sleep.wait(new QMutex(), sleepInterval);
		progress->setValue(i);
	}
}

void TestInvocationPlugin::doOperation(int sleepInterval, int count)
{
	for (int i = 0; i < count; ++i) {
		QWaitCondition sleep;
		sleep.wait(new QMutex(), sleepInterval);
	}
}

void TestInvocationPlugin::onOpertionFinished(invocation::InvocationState result)
{
	QString message = result == invocation::FinishedNormally ? "Finished" : "Terminated";
	qDebug() << message;
}
