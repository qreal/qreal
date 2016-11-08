#include <trikKitInterpreterCommon/trikEmulation/trikledadapter.h>

trik::TrikLedAdapter::TrikLedAdapter(trik::robotModel::parts::TrikLed *led) : mLed(led)
{
}

void trik::TrikLedAdapter::red()
{
	QMetaObject::invokeMethod(mLed, "setColor", Q_ARG(const QString &, "red"));
}

void trik::TrikLedAdapter::green()
{
	QMetaObject::invokeMethod(mLed, "setColor", Q_ARG(const QString &, "green"));
}

void trik::TrikLedAdapter::orange()
{
	QMetaObject::invokeMethod(mLed, "setColor", Q_ARG(const QString &, "orange"));
}

void trik::TrikLedAdapter::off()
{
	QMetaObject::invokeMethod(mLed, "setColor", Q_ARG(const QString &, "off"));
}
