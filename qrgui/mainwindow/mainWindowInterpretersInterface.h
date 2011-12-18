#pragma once

#include "../../qrkernel/ids.h"
#include "../toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace qReal {
namespace gui {

class ErrorReporter;

class MainWindowInterpretersInterface
{
public:
	virtual ~MainWindowInterpretersInterface() {}
	virtual void selectItem(Id const &graphicalId) = 0;
	virtual void highlight(Id const &graphicalId, bool exclusive = true) = 0;
	virtual void dehighlight(Id const &graphicalId) = 0;
	virtual void dehighlight() = 0;
	virtual ErrorReporterInterface *errorReporter() = 0;
	virtual Id activeDiagram() = 0;
	virtual void openSettingsDialog(QString const &tab) = 0;

	/// Save currently opened model
	virtual void saveAll() = 0;

	/// Opens new tab with text editor and shows a text in it
	/// @param title A title of the tab
	/// @param text A text that should be shown in an editor
	virtual void showInTextEditor(QString const &title, QString const &text) = 0;

	/// Rereads model information from repository and reinitializes models
	/// and all related views. Needs to be called after major changes in repo.
	virtual void reinitModels() = 0;
};

}
}
