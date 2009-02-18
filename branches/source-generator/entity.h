/** @file entity.h
 * 	@brief Описания вспомогательных классов сущностей
 * */
#ifndef ENTITY_H
#define ENTITY_H

#include <QStringList>
#include <QList>
#include <QPair>
#include <QDomElement>
#include <QDebug>

#define FOR_ALL_PROPERTIES(o,p) \
	for (QList<QPair<QString, QString> >::ConstIterator p = o->constPropBegin(); \
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

class Category;

// parent for nodes and edges class
/** @class Entity
 * 	@brief Абстрактная сущность
 * */
class Entity
{
	bool resolving_done;
	QList<QString> parents;
	QList<QPair<QString, QString> > properties;

protected:
	Category *cat;
	/** @brief Добавить свойство */
	void addProperty( QString name, /**< Название свойства */
	                  QString type /**< Тип свойства */
	                );
	/** @brief Добавить список родительских свойств */
	void addProperties(const QList< QPair<QString, QString> > &arg);

public:
	Entity(Category *category){ cat = category; visible = false; res = "\t<file>%1</file>\n"; resolving_done = false; width = -1; height = -1; }
	virtual ~Entity(){};

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

	QList<QPair<QString, QString> >::ConstIterator constPropBegin() const {return properties.constBegin();}
	QList<QPair<QString, QString> >::ConstIterator constPropEnd() const {return properties.constEnd();}
};

// node class
/** @class Node
 *  @brief Элемент
*/
class Node : public Entity
{
public:
	Node(Category *cat):Entity(cat) { type = NODE; }
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
	Edge(Category *cat):Entity(cat){
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

#endif
