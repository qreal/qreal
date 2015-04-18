#pragma once
#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class ChangeVersionWidget;
}

class ChangeVersionWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ChangeVersionWidget(QWidget *parent = 0);
	~ChangeVersionWidget();
	void updateLog(QList<QPair<QString, QString> > listLog);

signals:
	void loadVersion(QString hash);
	void showDiff(const QString &fstHash, const QString &scndHash, QWidget *widget);
	void swapTab();

private slots:
	void getHash();
	void showDiff(QListWidgetItem *item);

private:
	void initDiffWidget();
	void clearLayout(QLayout *layout);

	Ui::ChangeVersionWidget *mUi;
	QWidget *mDiffWidget;
};
