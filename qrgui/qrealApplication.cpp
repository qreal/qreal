#include "qrealApplication.h"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QWidget>

#include <qrkernel/settingsManager.h>
#include <qrutils/uxInfo/uxInfo.h>
#include <qrutils/virtualKeyboard.h>

using namespace qReal;

QRealApplication::QRealApplication(int &argc, char **argv)
	: QApplication(argc, argv)
{
	connect(this, &QRealApplication::focusChanged, this, &QRealApplication::onFocusChanged);
}

bool QRealApplication::notify(QObject *obj, QEvent *e)
{
	QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(e);

	QWidget *widget = dynamic_cast<QWidget*>(obj);
	QPoint pos;

	if (mouseEvent && widget && mouseEvent->type() == QMouseEvent::MouseButtonPress) {
		pos = widget->pos();
		for (; widget; widget = dynamic_cast<QWidget*>(widget->parent())) {
			pos += widget->pos();
		}

		utils::UXInfo::reportMouseClick(mouseEvent->pos() + pos);
	}

	return QApplication::notify(obj, e);
}

void QRealApplication::onFocusChanged(QWidget *old, QWidget *now)
{
	/// @todo: when osk.exe is called focus always returns to this process and calls it over and over again;
	/// need to memorize last focused widget.
	if (old != now && SettingsManager::value("touchMode").toBool()
			&& now && now->testAttribute(Qt::WA_InputMethodEnabled)) {
		utils::VirtualKeyboard::show();
	}
}
