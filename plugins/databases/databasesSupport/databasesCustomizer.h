#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QDockWidget>
#include <QIcon>
#include "../../../qrgui/toolPluginInterface/customizer.h"
#include <QObject>

namespace qReal {
namespace databasesSupport {

// TODO
/// Class for databases customisation
class DatabasesCustomizer : public QObject, public qReal::Customizer
{
	Q_OBJECT
public:
	~DatabasesCustomizer();

signals:

public slots:

};

}
}
