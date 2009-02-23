/** @file uml_guiobjectfactory.h
 * 	@brief Фабрика создания объектов на диаграммах
 * */
#pragma once

#include "../common/classes.h"

namespace UML {
	class Element;
	/** @brief Создать объект данного типа
	 *	@brief @return Объект данного типа
	 * */
	Element * GUIObjectFactory(TypeIdType const &type /**< Тип объекта */);
}
