#include "linuxFantom.h"
#include <QObject>

using namespace robotsInterpreter::robotCommunication;

bool Fantom::isAvailable()
{
	return false;
}

unsigned long Fantom::nFANTOM100_createNXT(char resString[], int status, unsigned char checkFVersion)
{
	Q_UNUSED(resString);
	Q_UNUSED(status);
	Q_UNUSED(checkFVersion);
	return 0;
}

void Fantom::nFANTOM100_iNXT_sendDirectCommand(unsigned long nxtHandle
		, bool requireResponse
		, const char *inputBufferPtr
		, int inputBufferSize
		, char *outputBufferPtr
		, int outputBufferSize
		, int &status)
{
	Q_UNUSED(nxtHandle);
	Q_UNUSED(requireResponse);
	Q_UNUSED(inputBufferPtr);
	Q_UNUSED(inputBufferSize);
	Q_UNUSED(outputBufferPtr);
	Q_UNUSED(outputBufferSize);
	Q_UNUSED(status);
}

unsigned long Fantom::nFANTOM100_createNXTIterator(unsigned char searchBluetooth, unsigned long bluetoothSearchTimeout, int &status)
{
	Q_UNUSED(searchBluetooth);
	Q_UNUSED(bluetoothSearchTimeout);
	Q_UNUSED(status);
	return 0;
}

void Fantom::nFANTOM100_iNXTIterator_getName(unsigned long NXTIterHandle, char resString[], int &status)
{
	Q_UNUSED(NXTIterHandle);
	Q_UNUSED(resString);
	Q_UNUSED(status);
}

unsigned long Fantom::nFANTOM100_iNXTIterator_getNXT(unsigned long nxtIterHandle, int &status)
{
	Q_UNUSED(nxtIterHandle);
	Q_UNUSED(status);
	return 0;
}

void Fantom::nFANTOM100_destroyNXTIterator(unsigned long nxtIteratorHandle, int &status)
{
	Q_UNUSED(nxtIteratorHandle);
	Q_UNUSED(status);
}

void Fantom::nFANTOM100_iNXTIterator_advance(unsigned long NXTIterHandle, int &status)
{
	Q_UNUSED(NXTIterHandle);
	Q_UNUSED(status);
}

void Fantom::nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode(char resString[], int &status)
{
	Q_UNUSED(resString);
	Q_UNUSED(status);
}
