#include "windowsFantom.h"
#include "robotCommunicationException.h"
#include "fantomMethods.h"

using namespace robotsInterpreter::robotCommunication;

Fantom::Fantom()
{
	mFantomLibrary.setFileName("fantom");
	mFantomLibrary.load();
}

bool Fantom::isAvailable()
{
	return mFantomLibrary.isLoaded();
}

unsigned long Fantom::nFANTOM100_createNXT(char resString[], int status, unsigned char checkFVersion)
{
	NFANTOM100_createNXT nFANTOM100_createNXTHandle = (NFANTOM100_createNXT)(mFantomLibrary.resolve("nFANTOM100_createNXT"));
	if (nFANTOM100_createNXTHandle)
		return nFANTOM100_createNXTHandle(resString, status, checkFVersion);
	else
		throw RobotCommunicationException("Fantom driver is not available");
}

void Fantom::nFANTOM100_iNXT_sendDirectCommand(unsigned long nxtHandle
		, bool requireResponse
		, const char *inputBufferPtr
		, int inputBufferSize
		, char *outputBufferPtr
		, int outputBufferSize
		, int &status)
{
	NFANTOM100_iNXT_sendDirectCommand nFANTOM100_iNXT_sendDirectCommandHandle = (NFANTOM100_iNXT_sendDirectCommand)(mFantomLibrary.resolve("nFANTOM100_iNXT_sendDirectCommand"));
	if (nFANTOM100_iNXT_sendDirectCommandHandle)
		nFANTOM100_iNXT_sendDirectCommandHandle(nxtHandle, requireResponse, inputBufferPtr, inputBufferSize, outputBufferPtr, outputBufferSize, status);
	else
		throw RobotCommunicationException("Fantom driver is not available");
}

unsigned long Fantom::nFANTOM100_createNXTIterator(unsigned char searchBluetooth, unsigned long bluetoothSearchTimeout, int &status)
{
	NFANTOM100_createNXTIterator nFANTOM100_createNXTIteratorHandle = (NFANTOM100_createNXTIterator)(mFantomLibrary.resolve("nFANTOM100_createNXTIterator"));
	if (nFANTOM100_createNXTIteratorHandle)
		return nFANTOM100_createNXTIteratorHandle(searchBluetooth, bluetoothSearchTimeout, status);
	else
		throw RobotCommunicationException("Fantom driver is not available");
}

void Fantom::nFANTOM100_iNXTIterator_getName(unsigned long NXTIterHandle, char resString[], int &status)
{
	NFANTOM100_iNXTIterator_getName nFANTOM100_iNXTIterator_getNameHandle = (NFANTOM100_iNXTIterator_getName)(mFantomLibrary.resolve("nFANTOM100_iNXTIterator_getName"));
	if (nFANTOM100_iNXTIterator_getNameHandle)
		nFANTOM100_iNXTIterator_getNameHandle(NXTIterHandle, resString, status);
	else
		throw RobotCommunicationException("Fantom driver is not available");
}

unsigned long Fantom::nFANTOM100_iNXTIterator_getNXT(unsigned long nxtIterHandle, int &status)
{
	NFANTOM100_iNXTIterator_getNXT nFANTOM100_iNXTIterator_getNXTHandle = (NFANTOM100_iNXTIterator_getNXT)(mFantomLibrary.resolve("nFANTOM100_iNXTIterator_getNXT"));
	if (nFANTOM100_iNXTIterator_getNXTHandle)
		return nFANTOM100_iNXTIterator_getNXTHandle(nxtIterHandle, status);
	else
		throw RobotCommunicationException("Fantom driver is not available");
}

void Fantom::nFANTOM100_destroyNXTIterator(unsigned long nxtIteratorHandle, int &status)
{
	NFANTOM100_destroyNXTIterator nFANTOM100_destroyNXTIteratorHandle = (NFANTOM100_destroyNXTIterator)(mFantomLibrary.resolve("nFANTOM100_destroyNXTIterator"));
	if (nFANTOM100_destroyNXTIteratorHandle)
		nFANTOM100_destroyNXTIteratorHandle(nxtIteratorHandle, status);
	else
		throw RobotCommunicationException("Fantom driver is not available");
}

void Fantom::nFANTOM100_iNXTIterator_advance(unsigned long NXTIterHandle, int &status)
{
	NFANTOM100_iNXTIterator_advance nFANTOM100_iNXTIterator_advanceHandle = (NFANTOM100_iNXTIterator_advance)(mFantomLibrary.resolve("nFANTOM100_iNXTIterator_advance"));
	if (nFANTOM100_iNXTIterator_advanceHandle)
		nFANTOM100_iNXTIterator_advanceHandle(NXTIterHandle, status);
	else
		throw RobotCommunicationException("Fantom driver is not available");
}

void Fantom::nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode(char resString[], int &status)
{
	NFANTOM100_iNXT_findDeviceInFirmwareDownloadMode nFANTOM100_iNXT_findDeviceInFirmwareDownloadModeHandle
			= (NFANTOM100_iNXT_findDeviceInFirmwareDownloadMode)(mFantomLibrary.resolve("nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode"));
	if (nFANTOM100_iNXT_findDeviceInFirmwareDownloadModeHandle)
		nFANTOM100_iNXT_findDeviceInFirmwareDownloadModeHandle(resString, status);
	else
		throw RobotCommunicationException("Fantom driver is not available");
}
