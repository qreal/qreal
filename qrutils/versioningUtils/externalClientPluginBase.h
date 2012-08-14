#pragma once

#include <QtCore/QProcess>

#include "../utilsDeclSpec.h"
#include "../../qrgui/versioning/versioningPluginInterface.h"
#include "boolCallback.h"

namespace qReal
{
namespace versioning
{

/// Base class for all VCS clients that implement their functionality
/// in some external process
class QRUTILS_EXPORT ExternalClientPluginBase : public VersioningPluginInterface
{
	Q_OBJECT

public:
	void setWorkingCopyManager(qrRepo::WorkingCopyManagementInterface *workingCopyManager);

	virtual void init(PluginConfigurator const &configurator);

	/// Starts process which executable`s path specified by setPathToClient() method and waits for its finish.
	/// @param args Startup arguments
	/// @param needPreparation Specifies if working copy must be fetchced from current project
	/// @param workingDir A path to working directory. If empty, tempFolder() used
	/// @param checkWorkingDir Specifies if before operation will be checked if specified directory is under version control
	/// @param needProcessing Specifies if changes in working copy must be registered in current project
	/// @param targetProject A path to project whicth will recieve chages in working copy after processing.
	///                      If empty value speified, target project will be working one
	/// @param sourceProject A path to source project. If empty working one used
	/// @param reportErrors Specifies if all occured errors are reported to GUI
	bool invokeOperation(QStringList const &args, bool needPreparation = true
			, QString const &workingDir = QString(), bool const checkWorkingDir = true
			, bool const needProcessing = true, QString const &targetProject = QString()
			, QString const &sourceProject = QString(), bool const reportErrors = true);

	/// Starts process which executable`s path specified by setPathToClient() method in separate thread.
	/// @param args Startup arguments
	/// @param needPreparation Specifies if working copy must be fetchced from current project
	/// @param callback A callback that will be called when operation complete with result specification.
	/// @param workingDir A path to working directory. If empty, tempFolder() used
	/// @param sourceProject A path to source project. If empty, working one used
	/// @param checkWorkingDir Specifies if before operation will be checked if specified directory is under version control
	/// @param reportErrors Specifies if all occured errors are reported to GUI
	/// @returns Pointer to started operaton
	invocation::LongOperation *invokeOperationAsync(QStringList const &args
		, invocation::BoolCallback *callback = 0
		, bool needPreparation = true
		, QString const &workingDir = QString()
		, QString const &sourceProject = QString()
		, bool const checkWorkingDir = true
		, bool reportErrors = true);

protected:
	ExternalClientPluginBase();
	virtual ~ExternalClientPluginBase() {}

	QString pathToClient() const;
	void setPathToClient(QString const &pathToClient);

	/// Returns current process standart output
	QString standartOutput() const;

	/// Guarantees the working copy existance in temp folder
	/// after this method is invoked
	void prepareWorkingCopy(QString const &workingDir, QString const &sourceProject);

	/// Notifies system about changes in working copy. Must be called after correcpoding VCS operations
	/// @param targetProject A path to project whicth will recieve chages in working copy after processing.
	///                      If empty value speified, target project will be working one
	void processWorkingCopy(QString const &targetProject = QString());

	/// Virtual, must be implemented in each plugin. Returns
	/// process communication timeout in ms
	virtual int timeout() const = 0;
	/// Virtual, must be implemented in each plugin. Returns
	/// path to folder where all temp operations must occure
	virtual QString tempFolder() const = 0;

protected slots:
	/// Must be called every time when occured error
	void onErrorOccured(QString const &errorMessage);
	/// Must be called every time when occured some errors
	void onErrorsOccured(QStringList const &errorMessages);

signals:
	/// The easiest way to invoke @see onErrorOccured() in on thread
	void errorOccured(QString const &message);


private slots:
	void onOperationComplete(invocation::LongOperation *operation);

private:
	bool startAndWait(const QStringList &args, bool reportErrors
			, QString const &workingDir, bool const checkWorkingCopy);

	bool startProcess(QStringList const &args, bool reportErrors = true);
	bool checkClientPath(bool reportErrors = true);
	bool processErrors(bool reportErrors = true);
	bool waitForClient(bool reportErrors = true);

	qrRepo::WorkingCopyManagementInterface *mWorkingCopyManager;
	gui::MainWindowInterpretersInterface *mMainWindow;
	ErrorReporterInterface *mErrorReporter;
	QString mPathToClient;
	QProcess *mClientProcess;
	QMap<invocation::LongOperation *, invocation::BoolCallback *> mRunningOperationsCallbacksMap;
};

}
}
