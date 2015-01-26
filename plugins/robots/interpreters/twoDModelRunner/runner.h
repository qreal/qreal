#pragma once

#include <interpreterCore/robotsPluginFacade.h>

namespace twoDModel {

class Runner
{
public:
	Runner();

	void interpret(const QString &saveFile);

private:
	interpreterCore::RobotsPluginFacade mPluginFacade;
};

}
