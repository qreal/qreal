#include "repairer.h"

#include <QDebug>

using namespace qReal;
using namespace model;

Repairer::Repairer(Model *model)
	: mModel(model), enabled(false)
{

}
