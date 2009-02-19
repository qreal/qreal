/** @file uml_guiobjectfactory.h
 * 	@brief Фабрика создания объектов на диаграммах
 * */
#ifndef UML_GUIOBJECTFACTORY_H
#define UML_GUIOBJECTFACTORY_H

namespace UML {
    class Element;
   	/** @brief Создать объект данного типа
	 *	@brief @return Объект данного типа
	 * */ 
    Element * GUIObjectFactory(int type /**< Тип объекта */);
};

#endif

