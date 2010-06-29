#pragma once

#include <QString>

namespace UML {

	/** @class ElementRepoIface
	 * 	@brief interface for getting data from the repo by plugins' classes
	 * */
	class ElementRepoInterface{
		public:
			virtual QString roleValueByName(QString const &roleName) const = 0;
			virtual QPersistentModelIndex index() const = 0;
	};
}
