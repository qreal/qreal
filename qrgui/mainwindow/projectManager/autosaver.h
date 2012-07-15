#pragma once
#include <QtCore/QObject>
#include <QtCore/QTimer>

namespace qReal {

class ProjectManager;

class Autosaver : public QObject
{
	Q_OBJECT

public:
	explicit Autosaver(ProjectManager *projectManager);
	void reinit();
	
private slots:
	void save();

private:
	uint interval();
	QString filePath();

	static uint const defaultInterval = 600;

	ProjectManager *mProjectManager;
	QTimer *mTimer;
};

}
