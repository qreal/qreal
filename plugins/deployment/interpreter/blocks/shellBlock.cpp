#include "shellBlock.h"

#include <QtCore/QProcess>
#include <QtCore/QDebug>

using namespace deployment::blocks;

ShellBlock::ShellBlock(ShellWidget *shellWidget)
	: mShellWidget(shellWidget)
{
}

void ShellBlock::run()
{
	QProcess * const process = new QProcess(this);
	connect(process, &QProcess::readyReadStandardOutput, [=]() {
		mShellWidget->append(qPrintable(process->readAllStandardOutput()));
	});
	connect(process, &QProcess::readyReadStandardError, [=]() {
		mShellWidget->append(qPrintable(process->readAllStandardError()));
	});
	connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=]() {
		delete sender();
		emit done(mNextBlockId);
	});
	process->start(processName(), arguments());
}

QString ShellBlock::processName() const
{
	return "bash";
}

QStringList ShellBlock::arguments()
{
	return {};
}
