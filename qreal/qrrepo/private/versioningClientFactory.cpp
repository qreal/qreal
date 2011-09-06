#include "versioningClientFactory.h"

using namespace qrRepo::details;

VersioningClientFactory::VersioningClientFactory(QString const &pathToClient)
{
	mSvnClient = new SvnClient(pathToClient);
	//TODO: Add other clients
}

qrRepo::VersionControlSystemInterface *VersioningClientFactory::GetClient()
{
	//TODO: Add switch between other clients
	return mSvnClient;
}
