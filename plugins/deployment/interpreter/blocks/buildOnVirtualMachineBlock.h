#pragma once

#include "shellBlock.h"

namespace deployment {
namespace blocks {

class BuildOnVirtualMachineBlock : public ShellBlock
{
public:
	explicit BuildOnVirtualMachineBlock(ShellWidget *shellWidget);

private:
	QStringList arguments() override;
};

}
}
