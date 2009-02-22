/** @file uml_guiobjectfactory.h
 * 	@brief Фабрика создания объектов на диаграммах
 * */
#ifndef UML_GUIOBJECTFACTORY_H
#define UML_GUIOBJECTFACTORY_H

#include "../common/classes.h"

namespace UML {
    class Element;
   	/** @brief Создать объект данного типа
	 *	@brief @return Объект данного типа
	 * */ 
    Element * GUIObjectFactory(TypeIdType type /**< Тип объекта */);
};

#endif

