#pragma once
#include "include/client.hpp"
#include "include/context_listener.hpp"
#include "include/targets.hpp"
#include <QtCore>
using namespace svn;

class SvnClient
{

public:
	Context *ctx;
	Targets *targets;
	Client client;

	SvnClient(char *workDir, char *username, char *pass);
	int commit(QString &message);
	void checkout();
//private:
};
