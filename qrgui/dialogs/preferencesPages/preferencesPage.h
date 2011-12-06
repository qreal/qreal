#pragma once

#include <QtGui/QIcon>
#include <QWidget>

/**
 * @abstract
 * @brief Abstract class for preferences page
 */
class PreferencesPage : public QWidget
{
	Q_OBJECT
	friend class PreferencesDialog;

public:
	explicit PreferencesPage(QWidget *parent = 0) : QWidget(parent){}
	virtual ~PreferencesPage(){}
	/** @brief This method will be called on pressing "Apply" or "Ok" */
	virtual void save() = 0;
	virtual QIcon getIcon() = 0;

};
