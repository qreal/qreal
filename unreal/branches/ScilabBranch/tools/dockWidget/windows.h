#ifndef WINDOWS_H
#define WINDOWS_H

#include <QMainWindow>
#include <QtGui>
#include <QDebug.h>
#define PANEL_WIDTH 142
#define PANEL_MARGIN 12

namespace Ui {
	class Windows;
}

class Windows : public QMainWindow {
	Q_OBJECT
public:
	Windows(QWidget *parent = 0);
	~Windows();

protected:
	void changeEvent(QEvent *e);

	void resizeEvent(QResizeEvent*);
private:
	Ui::Windows *ui;
	QTimeLine *m_timeLine;
	int startX;
	int startY;

public slots:

	void changeState();

private slots:

	void setSlidePosition(int pos);
	void buttonPressed();

	//void location();
};


#endif // WINDOWS_H
