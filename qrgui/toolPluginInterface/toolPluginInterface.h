#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QString>

namespace qReal {

class ToolPluginInterface
{
public:
};

}

Q_DECLARE_INTERFACE(qReal::ToolPluginInterface, "ru.tepkom.QReal.ToolPluginInterface/0.1")
