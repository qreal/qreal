#include "debugHelper.h"

using namespace qReal::interpreters::robots;
using namespace qReal::interpreters::robots::details;

QString DebugHelper::toString(robotModelType::robotModelTypeEnum const &modelType)
{
	switch (modelType)
	{
	case robotModelType::null:
		return "null";
	case robotModelType::real:
		return "real";
	case robotModelType::unreal:
		return "d2Model";
	}
	Q_ASSERT(!"Incorrect model type");
	return "Incorrect model type";
}
