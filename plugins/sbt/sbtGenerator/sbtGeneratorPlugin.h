#pragma once

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

class SbtGeneratorPlugin : public QObject, public qReal::ToolPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(qReal::ToolPluginInterface)

public:
    SbtGeneratorPlugin();
    virtual ~SbtGeneratorPlugin();

    /// Initializes generator with ref to logical model.
    virtual void init(qReal::PluginConfigurator const &configurator);

    /// Returns list containing "generate" action, which runs a generator.
    virtual QList<qReal::ActionInfo> actions();

    /// Provides customization object that changes main window title and so on.
    //virtual qReal::Customizer* customizationInterface();

private:
    //	/// Customization object for configuring QReal main window.
    //	Customizer mCustomizer;

    //	/// Generator main object, directs generation.
    //	Generator mGenerator;
};

