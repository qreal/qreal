#include "umlPlugin.h"
using namespace qReal;
using namespace uml;

UmlPlugin::UmlPlugin()
{}

qReal::Customizer * UmlPlugin::customizationInterface()
{
	return &mCustomizer;
}
