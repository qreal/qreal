#pragma once

#include "shellBlock.h"

namespace deployment {
namespace blocks {

class HelpWithChangelogBlock : public ShellBlock
{
public:
	explicit HelpWithChangelogBlock(ShellWidget *shellWidget);

private:
	QStringList arguments() override;
};

}
}
