#include "debugHelper.h"

using namespace qReal::interpreters::robots;
using namespace qReal::interpreters::robots::details;

QString DebugHelper::toString(robotModelType::robotModelTypeEnum const &modelType)
{
	switch (modelType)
	{
	case robotModelType::null:
		return "null";
	case robotModelType::twoD:
		return "twoD";
	case robotModelType::nxt:
		return "legoNxt";
	case robotModelType::trik:
		return "trik";
	}
	Q_ASSERT(!"Incorrect model type");
	return "Incorrect model type";
}
