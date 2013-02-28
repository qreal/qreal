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
	explicit PythonInterpreter(QObject *parent
			, QString const &pythonPath = "python"
			, QString const &reactionScriptPath = QDir().currentPath() + "/reaction.py"
			, QString const &applicationConditionScriptPath = QDir().currentPath() + "/appcond.py");
	~PythonInterpreter();

	/// Interpret reaction or application condition python script
	bool interpret(bool const isApplicationCondition);

	/// Interpret specific code, represented as string
	void interpretCode(QString const code);

	void terminateProcess();

	void setPythonPath(QString const &path);
	void setReactionScriptPath(QString const &path);
	void setApplicationConditionScriptPath(QString const &path);

signals:
	/// Emitted after parsing std output and has all properties changes
	void readyReadStdOutput(QHash<QPair<QString, QString>, QString> const &output);

	/// Raw error output from python interpreter
	void readyReadErrOutput(QString const &output);

private slots:
	/// Read python interpreter std output
	void readOutput();

	/// Read python interpreter error output
	void readErrOutput();

private:
	bool startPythonInterpreterProcess();

	/// Parses interpreter std output and returns new values for element properties
	QHash<QPair<QString, QString>, QString> &parseOutput(QString const &output) const;

	/// Moves along the output and accumulates properties changes
	void parseOutput(QHash<QPair<QString, QString>, QString> &res, QString const &output, int &pos) const;

	QThread *mThread;
	QProcess *mInterpreterProcess;

	QString mPythonPath;
	QString mReactionScriptPath;
	QString mApplicationConditionScriptPath;

	bool mApplicationConditionResult;
};

}
