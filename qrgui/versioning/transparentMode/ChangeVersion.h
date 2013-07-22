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
	void swapTab();

public slots:
	void updateLog(QList<QPair<QString, QString> > listLog);

private slots:
	void obtainHash(QListWidgetItem* item);

private:
	Ui::ChangeVersion *mUi;
};
