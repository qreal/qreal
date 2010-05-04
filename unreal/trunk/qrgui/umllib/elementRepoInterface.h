#pragma once

#include <QString>

namespace UML {

	/** @class ElementRepoIface
	 * 	@brief Интерфейс для получения данных из репо генерящимися в плагины классами
	 * */
	class ElementRepoInterface{
		public:
			virtual QString roleValueByName(QString const &roleName) const = 0;
			virtual QPersistentModelIndex index() const = 0;
	};
}
