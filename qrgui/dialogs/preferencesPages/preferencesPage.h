#pragma once

#include <QtGui/QIcon>
#include <QtWidgets/QWidget>

/// Abstract class for preferences page
class PreferencesPage : public QWidget
{
	Q_OBJECT

	/// TODO: friend to be able to call protected changeEvent() method. It seems to be bad idea.
	friend class PreferencesDialog;

public:
	explicit PreferencesPage(QWidget *parent = 0) : QWidget(parent)
	{
	}

	virtual ~PreferencesPage()
	{
	}

	/// This method will be called on pressing "Apply" or "Ok"
	virtual void save() = 0;
	virtual void restoreSettings() = 0;

	/// This method will be called when need to get icon on label in form
	virtual QIcon getIcon() const
	{
		return mIcon;
	}

protected:
	/// An icon to be shown near this tab in preferences window
	QIcon mIcon;

};
