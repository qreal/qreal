#ifndef REFWINDOW_H
#define REFWINDOW_H

#include <QWidget>
#include "../../qrrepo/repoApi.h"

namespace Ui {
	class RefWindow;
}

class RefWindow : public QWidget
{
	Q_OBJECT

public:
	explicit RefWindow(const qrRepo::RepoApi *mApi, QString name, QWidget *parent = 0);
	~RefWindow();

private:
	Ui::RefWindow *ui;
	const qrRepo::RepoApi *api;
	QString typeName;
};

#endif // REFWINDOW_H
