#pragma once
#include <QWidget>
#include <QListWidgetItem>
#include "../transparentMode/transparentMode.h"

namespace Ui {
class ChangeVersion;
}

class ChangeVersion : public QWidget
{
	Q_OBJECT
	
public:
	explicit ChangeVersion(QWidget *parent = 0);
	~ChangeVersion();

signals:
	void hashObtained(QString hash);
	void showDiff();
	void showDiff(QString, QString, QWidget*);
	void swapTab();

public slots:
	void updateLog(QList<QPair<QString, QString> > listLog);
	void onViewForTransparentModeIsReady(QGraphicsView* mView);


private slots:
	void obtainHash();
	void showDiff(QListWidgetItem* item);

private:
	Ui::ChangeVersion *mUi;
};
