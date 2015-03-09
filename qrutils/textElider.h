#pragma once

#include <QtWidgets/QWidget>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// Provides text auto-trimming on a given widget.
/// The given widget must have setText(QString) slot, i.e. QLabel is ok
class QRUTILS_EXPORT TextElider: public QObject
{
	Q_OBJECT

public:
	explicit TextElider(QWidget *widget, Qt::TextElideMode mode = Qt::ElideRight);

signals:
	/// Emitted after each time after text recalculation
	void textChanged(const QString &text);

public slots:
	void setText(const QString &text);

private:
	QWidget *mWidget;  // Doesn`t take ownership
	Qt::TextElideMode mMode;
};

}
