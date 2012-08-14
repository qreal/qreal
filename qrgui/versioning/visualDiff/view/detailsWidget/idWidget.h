#pragma once

#include <QtCore/QEvent>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>

#include "../../../../qrkernel/ids.h"

namespace versioning
{
namespace details
{

class IdWidget : public QWidget
{
	Q_OBJECT

public:
	IdWidget(QString const &defaultText, QWidget *parent = 0);

	void setId(qReal::Id const &graphicalId, qReal::Id const &logicalId);
	void reset();

protected:
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);

signals:
	void mouseEntered(qReal::Id const& id);
	void mouseLeaved(qReal::Id const& id);

private:
	QString labelText() const;

	QString mDefaultText;
	bool mIdSetted;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;
	QGridLayout *mLayout;
	QLabel *mLabel;

};

}
}
