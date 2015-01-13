#pragma once

#include "shellBlock.h"

namespace deployment {
namespace blocks {

class IncrementVersionInSourceCodeBlock : public ShellBlock
{
public:
	explicit IncrementVersionInSourceCodeBlock(ShellWidget *shellWidget);

private:
	QStringList arguments() override;
};

}
}
