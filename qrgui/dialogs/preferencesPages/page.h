#pragma once

#include <QWidget>

class PreferencesPage : public QWidget
{
	Q_OBJECT
	friend class PreferencesDialog;

public:
	explicit PreferencesPage(QWidget *parent = 0) : QWidget(parent){}
	virtual void save() = 0;

};
