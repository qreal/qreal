#pragma once

#include "../../../qrgui/toolPluginInterface/customizer.h"

namespace qReal {
namespace interpreters {
namespace robots {

class Customizer : public qReal::Customizer
{
public:
	virtual bool showLogicalModelExplorer() const
	{
		return true;
	}

	virtual bool showLibraryRelatedMenus() const
	{
		return true;
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
