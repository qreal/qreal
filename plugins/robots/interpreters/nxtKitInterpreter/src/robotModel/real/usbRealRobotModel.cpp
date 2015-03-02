#include "usbRealRobotModel.h"

#include "communication/usbRobotCommunicationThread.h"

using namespace nxt::robotModel::real;

UsbRealRobotModel::UsbRealRobotModel(const QString &kitId, const QString &robotId)
	: RealRobotModel(kitId, robotId, new communication::UsbRobotCommunicationThread)
{
}

QString UsbRealRobotModel::name() const
{
	return "NxtUsbRealRobotModel";
}

QString UsbRealRobotModel::friendlyName() const
{
	return tr("Interpretation (USB)");
}
