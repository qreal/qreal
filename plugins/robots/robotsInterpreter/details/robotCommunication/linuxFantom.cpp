#include "linuxFantom.h"

using namespace robotsInterpreter::robotCommunication;

bool Fantom::isAvailable()
{
	return false;
}

unsigned long Fantom::nFANTOM100_createNXT(char resString[], int status, unsigned char checkFVersion)
{
}

void Fantom::nFANTOM100_iNXT_sendDirectCommand(unsigned long nxtHandle
		, bool requireResponse
		, const char *inputBufferPtr
		, int inputBufferSize
		, char *outputBufferPtr
		, int outputBufferSize
		, int &status)
{
}

unsigned long Fantom::nFANTOM100_createNXTIterator(unsigned char searchBluetooth, unsigned long bluetoothSearchTimeout, int &status)
{
}

void Fantom::nFANTOM100_iNXTIterator_getName(unsigned long NXTIterHandle, char resString[], int &status)
{
}

unsigned long Fantom::nFANTOM100_iNXTIterator_getNXT(unsigned long nxtIterHandle, int &status)
{
}

void Fantom::nFANTOM100_destroyNXTIterator(unsigned long nxtIteratorHandle, int &status)
{
}

void Fantom::nFANTOM100_iNXTIterator_advance(unsigned long NXTIterHandle, int &status)
{
}

void Fantom::nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode(char resString[], int &status)
{
}
