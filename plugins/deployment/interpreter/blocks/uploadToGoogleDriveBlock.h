#pragma once

#include "shellBlock.h"

namespace deployment {
namespace blocks {

class UploadToGoogleDriveBlock : public ShellBlock
{
public:
	explicit UploadToGoogleDriveBlock(ShellWidget *shellWidget);

private:
	QStringList arguments() override;
};

}
}
