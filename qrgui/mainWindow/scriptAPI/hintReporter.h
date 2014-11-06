#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

namespace qReal {

namespace gui {

/// Represents hint reporter messages. Allows draw hint widgets with opacity animation (for a lifeTime).
/// Class HintAPI provides interface to send those messages in any window of the system.
class HintReporter : public QLabel
{
	Q_OBJECT

public:
	HintReporter(QWidget *parent, QString const &message, int const lifeTime);

protected:
	virtual void mouseReleaseEvent(QMouseEvent *event);

private slots:
	void disappear();

private:
	void addHint(QString const &hint);

	int mDuration;
};

}
}
