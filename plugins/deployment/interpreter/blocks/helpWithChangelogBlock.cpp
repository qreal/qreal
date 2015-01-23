#include "helpWithChangelogBlock.h"

#include <QtCore/QCoreApplication>

using namespace deployment::blocks;

HelpWithChangelogBlock::HelpWithChangelogBlock(ShellWidget *shellWidget)
	: ShellBlock(shellWidget)
{
}

QStringList HelpWithChangelogBlock::arguments()
{
	return {
		QCoreApplication::applicationDirPath() + "/deployment-scripts/version_increment/generate_changelog.sh"
		, stringProperty(id(), "Path")
		, stringProperty(id(), "Tag")
	};
}
