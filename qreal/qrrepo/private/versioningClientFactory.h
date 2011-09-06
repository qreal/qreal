#pragma once

#include "../versionControlSystemInterface.h"
#include "clients/svnClient.h"

namespace qrRepo
{
namespace details
{

class VersioningClientFactory
{
public:
	VersioningClientFactory(QString const &pathToClient);
	VersionControlSystemInterface *GetClient();

private:
	SvnClient *mSvnClient;
};

}
}
