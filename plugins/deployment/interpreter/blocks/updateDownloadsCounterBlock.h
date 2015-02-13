#pragma once

#include "shellBlock.h"

namespace deployment {
namespace blocks {

class UpdateDownloadsCounterBlock : public ShellBlock
{
public:
	explicit UpdateDownloadsCounterBlock(ShellWidget *shellWidget);

private:
	QStringList arguments() override;
};

}
}
