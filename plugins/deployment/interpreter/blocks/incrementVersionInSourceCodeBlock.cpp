#include "incrementVersionInSourceCodeBlock.h"

#include <QtCore/QCoreApplication>

using namespace deployment::blocks;

IncrementVersionInSourceCodeBlock::IncrementVersionInSourceCodeBlock(ShellWidget *shellWidget)
	: ShellBlock(shellWidget)
{
}

QStringList IncrementVersionInSourceCodeBlock::arguments()
{
	return {
		QCoreApplication::applicationDirPath() + "/deployment-scripts/version_increment/increment_trik_studio.sh"
		, eval<QString>("Path")
		, eval<QString>("OldVersion")
		, eval<QString>("NewVersion")
	};
}
