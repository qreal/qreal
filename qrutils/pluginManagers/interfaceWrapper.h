#pragma once

#include <QtCore/QObject>

#include <qrutils/utilsDeclSpec.h>

namespace qReal {

template <class InterfaceType>
class QRUTILS_EXPORT InterfaceWrapper
{
public:
	/// wraps one object to interface
	static InterfaceType *wrappedInterface(QObject *interfaceToWrap)
	{
		InterfaceType *castedInterface = qobject_cast<InterfaceType *>(interfaceToWrap);

		if (castedInterface) {
			return castedInterface;
		} else {
			return NULL;
		}
	}

	/// wraps list of objects
	static QList<InterfaceType *> listOfInterfaces(QList<QObject *> interfacesToWrap)
	{
		QList<InterfaceType *> interfacesList;

		foreach (QObject const *currentInterface, interfacesToWrap) {
			InterfaceType *castedInterface = qobject_cast<InterfaceType *>(currentInterface);
			if (castedInterface) {
				interfacesList.append(castedInterface);
			}
		}

		return interfacesList;
	}
};
}
