#include "bluetoothRealRobotModel.h"

#include "communication/bluetoothRobotCommunicationThread.h"

using namespace nxt::robotModel::real;

BluetoothRealRobotModel::BluetoothRealRobotModel(const QString &kitId, const QString &robotId)
	: RealRobotModel(kitId, robotId, new communication::BluetoothRobotCommunicationThread)
{
}

QString BluetoothRealRobotModel::name() const
{
	return "NxtBluetoothRealRobotModel";
}

QString BluetoothRealRobotModel::friendlyName() const
{
	return tr("Interpretation (Bluetooth)");
}
