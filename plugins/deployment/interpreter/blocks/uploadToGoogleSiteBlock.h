#pragma once

#include "shellBlock.h"

namespace deployment {
namespace blocks {

class UploadToGoogleSiteBlock : public ShellBlock
{
public:
	explicit UploadToGoogleSiteBlock(ShellWidget *shellWidget);

private:
	QString processName() const override;
	QStringList arguments() override;
};

}
}
