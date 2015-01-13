#include "shellBlock.h"

using namespace deployment::blocks;

ShellBlock::ShellBlock(ShellWidget *shellWidget)
	: mProcess(new QProcess(this))
	, mShellWidget(shellWidget)
{
	connect(mProcess, &QProcess::readyReadStandardOutput, [=]() {
		mLastOutput = mProcess->readAllStandardOutput();
		mShellWidget->append(mLastOutput);
	});
	connect(mProcess, &QProcess::readyReadStandardError, [=]() {
		mLastError = mProcess->readAllStandardError();
		mShellWidget->append(mLastError);
	});
	connect(mProcess, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=]() {
		delete sender();
		emit done(mNextBlockId);
	});
}

void ShellBlock::run()
{
	mProcess->start(processName(), arguments());
}

QString ShellBlock::processName() const
{
	return "bash";
}

QStringList ShellBlock::arguments()
{
	return {};
}
