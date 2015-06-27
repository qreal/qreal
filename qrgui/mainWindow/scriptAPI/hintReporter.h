#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

namespace qReal {
namespace gui {

/// Nice looking bubbling messages. Shown upon the given widget for the half of the given time, then fades away.
/// HTML may be passed there. Clicking disables message before the lifetime goes out.
class HintReporter : public QLabel
{
	Q_OBJECT

public:
	HintReporter(QWidget *parent, const QString &message, int lifeTime);

private slots:
	void disappear();

private:
	void mouseReleaseEvent(QMouseEvent *event) override;

	int mDuration;
};

}
}
