#pragma once
#include <QtCore/QObject>
#include <QtCore/QTimer>

namespace qReal {

class ProjectManager;

/**
 * @brief class provides automatic saving of the project at equal time intervals.
 * All options for working are retrieved from the settings manager.
 *
 * Autosaver provides an interface that allows you to make it reload parameters
 */
class Autosaver : public QObject
{
	Q_OBJECT

public:
	class Pauser
	{
	public:
		Pauser(Autosaver *autosaver);
		~Pauser();

	private:
		Autosaver *mAutosaver;
	};

	explicit Autosaver(ProjectManager *projectManager);
	void reinit();
	void stop();
	QString filePath();
	Pauser pauser();

private slots:
	void save();

private:
	uint interval();

	// if option AutosaveInterval in not specified
	static uint const defaultInterval = 600;

	ProjectManager *mProjectManager;
	QTimer *mTimer;
};

}
