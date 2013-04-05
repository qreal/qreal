#pragma once

#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QProcess>
#include <QtCore/QPair>
#include <QtCore/QHash>

namespace qReal {

/// Interprets reaction python script in different python interpreter process, parses output
/// and sends it to the main system
class PythonInterpreter : public QObject
{
	Q_OBJECT

public:
	enum CodeType {
		initialization,
		applicationCondition,
		reaction
	};

public:
	explicit PythonInterpreter(QObject *parent, QString const &pythonPath = "python"
			, QString const &tempScriptPath = QDir().currentPath() + "/temp.py");
	~PythonInterpreter();

	/// Interpret python script
	bool interpret(QString const &code, CodeType const codeType);

	void terminateProcess();
	void continueStep();

	/// Delete generated python script file
	void deleteTempFile();

	void setPythonPath(QString const &path);
	void setTempScriptPath(QString const &path);

signals:
	/// Emitted after parsing std output and has all properties changes
	void readyReadStdOutput(QHash<QPair<QString, QString>, QString> const &output);

	/// Raw error output from python interpreter
	void readyReadErrOutput(QString const &output);

private slots:
	/// Read python interpreter std output
	void readOutput();

private:
	bool startPythonInterpreterProcess();

	/// Parses interpreter std output and returns new values for element properties
	QHash<QPair<QString, QString>, QString> &parseOutput(QString const &output) const;

	/// Moves along the output and accumulates properties changes
	void parseOutput(QHash<QPair<QString, QString>, QString> &res, QString const &output, int &pos) const;

	QThread *mThread;
	QProcess *mInterpreterProcess;

	QString mPythonPath;
	QString mTempScriptPath;

	bool mPythonCodeProcessed;
	bool mApplicationConditionResult;
	bool mErrorOccured;
};

}
