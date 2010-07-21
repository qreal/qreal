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

	SvnClient(char *workDir);//, char *username, char *pass);
	long commit(QString &message);
	QString checkout(char *pathFrom, svn::Path pathTo);
	void auth(char *username, char *pass);
//private:
};
