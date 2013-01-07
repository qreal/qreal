#pragma once

#include <QtCore/QList>
#include <QtGui/QFrame>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <QtGui/QStackedLayout>

#include "../utilsDeclSpec.h"

namespace navigation
{

/// An engine for declarative menu specification
class QRUTILS_EXPORT NavigationMenuContent
{
public:
	NavigationMenuContent();
	/// Clones the content without cloning buttons and stretches
	virtual NavigationMenuContent *clone();

	void appendButton(QPushButton *button);
	void appendButtons(QList<QPushButton *> const &buttons);
	void insertButton(QPushButton *button, int index);
	void insertButtons(QList<QPushButton *> const &buttons, int index);
	void addStretch();
	void insertStretch(int index);
	// TODO: implement removing

	/// Returns the layout with given buttons and stretches
	QBoxLayout *toLayout();

private:
	QBoxLayout *mLayout;
};

class QRUTILS_EXPORT NavigationMenu : public QFrame
{
public:
	NavigationMenu();
	/// Sets the menu content to specified one. If 0 passed menu becomes hidden
	void setContent(NavigationMenuContent *content);

private:
	int indexOf(QLayout *layout);

	QStackedLayout *mLayout;
};

}
