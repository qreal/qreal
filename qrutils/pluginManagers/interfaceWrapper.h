#pragma once

#include <QtCore/QObject>

namespace qReal {

template <class InterfaceType>
class InterfaceWrapper
{
public:
	/// wraps one qobject to interface
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
