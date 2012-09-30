#pragma once

#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QProcess>
#include <QtCore/QPair>
#include <QtCore/QHash>

namespace qReal {

class PythonInterpreter : public QObject
{
	Q_OBJECT

public:
	explicit PythonInterpreter(QObject *parent
			, QString const pythonPath = "python"
			, QString const scriptPath = QDir().currentPath() + "/reaction.py");
	~PythonInterpreter();

	void interpret();

	void setPythonPath(QString const &path);
	void setScriptPath(QString const &path);

signals:
	void readyReadStdOutput(QHash<QPair<QString, QString>, QString> const &output);
	void readyReadErrOutput(QString const &output);

private slots:
	void readOutput();
	void readErrOutput();

private:
	QHash<QPair<QString, QString>, QString> &parseOutput(QString const &output) const;
	void parseOutput(QHash<QPair<QString, QString>, QString> &res, QString const &output, int &pos) const;

	QThread *mThread;
	QProcess *mInterpreterProcess;
	QString mPythonPath;
	QString mScriptPath;
};

}
