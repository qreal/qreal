#pragma once

#include "../../../qrgui/toolPluginInterface/customizationInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {

class Customizer : public CustomizationInterface
{
public:
	virtual bool showLogicalModelExplorer() const
	{
		return false;
	}

	virtual QString windowTitle() const
	{
		return QObject::tr("QReal:Robots");
	}

	virtual QIcon applicationIcon() const
	{
		return QIcon(":/icons/kroki.png");
	}

};

}
}
}
