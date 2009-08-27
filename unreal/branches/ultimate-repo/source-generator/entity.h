/** @file entity.h
 * 	@brief Описания вспомогательных классов сущностей
 * */

#pragma once

#include <QStringList>
#include <QList>
#include <QPair>
#include <QDomElement>
#include <QDebug>

#include "property.h"

#define FOR_ALL_PROPERTIES(o,p) \
	for (QList<RealProperty*>::ConstIterator p = o->constPropBegin(); \
	     p != o->constPropEnd(); p++)

class Generator;
extern QString resources;

/** @brief Тип сущности */
enum elementsType {
	NODE, /**< Элемент */
	EDGE /**< Связь */
};

/**< Тип конца связи */
enum sideType {
	BEGIN, /**< Начало */
	END /**< Конец */
};

class Port;
class Label;

class Editor;

// parent for nodes and edges class
/** @class Entity
 * 	@brief Абстрактная сущность
 * */
class Entity
{
	bool resolving_done;
	QList<RealProperty *> properties; // All properties, including inherited
	QList<RealProperty *> own_properties; // This own properties

protected:
	Editor *cat;
	QList<QString> parents;
	/** @brief Добавить свойство */
	bool addProperty(RealProperty *);
	bool addParentProperties(const Entity*);

public:
	Entity(Editor *category){ cat = category; visible = false; res = "\t<file>%1</file>\n"; resolving_done = false; width = -1; height = -1; }
	virtual ~Entity();

	virtual bool init(QDomElement &) = 0;
	bool resolve(void);
	bool isResolved(void) const {return resolving_done;}
	bool parseGeneralizations(QDomElement&);
	bool parseProperties(QDomElement&);
	bool parseAssociations(QDomElement&);
	bool parseLabels(QDomElement&);
	bool applyParent(const Entity *);

	/** @brief Высота графического представления сущности по умолчанию */
	int height;
	/** @brief Ширина графического представления сущности по умолчанию */
	int width;

	/** @brief Идентификатор */
	QString id;
	/** @brief Имя */
	QString name;
	/** @brief Список текстовых надписей, параметризующих SVG */
	QList< Label > labels;

	/** @brief Тип сущности */
	elementsType type;

	/** @brief Индикатор визуальности сущности */
	bool visible;

	QString res;

	QList<RealProperty*>::ConstIterator constPropBegin() const {return properties.constBegin();}
	QList<RealProperty*>::ConstIterator constPropEnd() const {return properties.constEnd();}
	QDomElement elem;
};

// node class
/** @class Node
 *  @brief Элемент
*/
class Node : public Entity
{
public:
	Node(Editor *cat):Entity(cat) { type = NODE; }
	~Node() {}
	//TODO: containers 
	//QStringList associations; 
	/** @brief Список портов */
	QList< Port > ports;

	bool init(QDomElement &);
	bool parseSdf(QDomElement&);
	bool parsePorts(QDomElement&);
};

/** @class Association
 * 	@brief Ассоциация */
class Association {
public:
	Association() { toArrow = ""; fromArrow = ""; }

	/** @brief Идентификатор начала ассоциации */
	QString fromID;
	/** @brief Идентификатор конца ассоциации */
	QString toID;
	/** @brief Идентификатор типа элементов, к которым можно присоединять начало ассоциации */
	QString from;


      /** @brief Идентификатор типа элементов, к которым можно присоединять конец ассоциации */
	QString to;
	/** @brief Тип стрелки начала ассоциации */
	QString fromArrow;
	/** @brief Тип стрелки конца ассоциации */
	QString toArrow;
    //TODO: full schema support
	//QString role;  
};

/** @class Port
 * 	@brief Порт элемента
*/
class Port {
public:
	/** @brief Тип порта */
	QString type;
	/** @brief Координаты порта */
	QList< qreal > vals;
        QList< bool > props;

};

/** @class @Label
 * 	@brief Надпись, параметризующая визуальное представление элемента в формате SVG 
*/
class Label{
public:
	/** @brief Координата х */
	qreal x;
	/** @brief Координата y */
	qreal y;
	/** @brief Надпись */
	QString text;
	/** @brief Роль */
	QStringList args;
};


// edge class
/** @class Edge
 *	@brief Связь
*/
class Edge : public Entity
{
public:
	Edge(Editor *ed):Entity(ed){
		assocsPropagated = false;
		endsPropagated   = false;
		lineType = "Qt::SolidLine";
		type = EDGE;
	}
	~Edge(){
		while (!associations.isEmpty())
			delete associations.takeFirst();
	}

	/** @brief Добавить описание ассоциации */
	void addAssociation( Association* arg /**< Ассоциация */);

	/** @brief Список ассоциаций */
	QList< Association*  > associations;
	/** @brief Тип линии */
	QString lineType;

	/** @brief Вспомогательная переменная, определяющая, обработан ли уже список ассоциаций */
	bool assocsPropagated; // edges' own generalizations
	/** @brief Вспомогательная переменная, определяющая, обработан ли уже список элементов, к которым может присоединять связь*/
	bool endsPropagated; // connected nodes' generalizations

	bool init(QDomElement&);
	bool parseEdgeGraphics(QDomElement&);
};
