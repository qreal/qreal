/** @file entity.h
 * 	@brief Описания вспомогательных классов сущностей
 * */
#ifndef ENTITY_H
#define ENTITY_H

#include <QStringList>
#include <QList>
#include <QPair>
#include <QDebug>

/** @brief Тип сущности */
enum elementsType { NODE, /**< Элемент */
					EDGE /**< Связь */
					};

/**< Тип конца связи */
enum sideType { BEGIN, /**< Начало */
				END /**< Конец */
				};

class Port;
class Label;

// parent for nodes and edges class
/** @class Entity
 * 	@brief Абстрактная сущность
 * */
class Entity
{
public:
    Entity(){ propsPropagated = false; parentsPropagated = false; visible = false; }
    virtual ~Entity(){};

	/** @brief Добавить родителя */
    void addParent( QString arg /**< Идентификатор родителя */);
	/** @brief Добавить дочерний элемент */
    void addChild( QString arg /**< Идентификатор дочернего элемента */);
	/** @brief Добавить свойство */
    void addProperty( QString name, /**< Название свойства */
						QString type /**< Тип свойства */
						);

	/** @brief Высота графического представления сущности по умолчанию */
    int height; 
	/** @brief Ширина графического представления сущности по умолчанию */
    int width;

	/** @brief Идентификатор */
    QString id;
	/** @brief Имя */
    QString name;
	/** @brief Список родителей */
    QStringList parents;
	/** @brief Список дочерних элементов */
    QStringList children;

	/** @brief Свойства */
    QList< QPair<QString, QString> > properties;

	/** @brief Список текстовых надписей, параметризующих SVG */
    QList< Label > labels;

	/** @brief Тип сущности */
    elementsType type;

	/** @brief Индикатор визуальности сущности */
    bool visible;

	/** @brief Вспомогательная переменная, определяющая, обработан ли уже список свойств */
    bool propsPropagated;
	/** @brief Вспомогательная переменная, определяющая, обработан ли уже список родителей */
    bool parentsPropagated;
};

// node class
/** @class Node
 *  @brief Элемент
*/
class Node : public Entity
{
public:
    Node() { type = NODE; }
    ~Node() {}
    //TODO: containers 
    //QStringList associations; 
	/** @brief Список портов */
    QList< Port > ports;   
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
    Edge(){
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
};



#endif
