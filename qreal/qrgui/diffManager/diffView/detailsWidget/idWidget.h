#pragma once

#include "../../../kernel/ids.h"

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QEvent>

namespace qReal
{
namespace diffManager
{
namespace diffView
{
namespace detailsWidget
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
	QString mDefaultText;
	bool mIdSetted;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;
	QGridLayout *mLayout;
	QLabel *mLabel;

	QString labelText() const;
};

}
}
}
}
