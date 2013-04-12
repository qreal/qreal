#pragma once


#include <QtWidgets/QWidget>
#include <QtXml/QDomDocument>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QtGui/QIconEngine>

class SdfRenderer : public QObject
{
	Q_OBJECT

public:
	SdfRenderer();
	SdfRenderer(QString const path);
	SdfRenderer(const QDomNode &bla);
	~SdfRenderer();

	bool load (QString const &filename);
	QString render();
	void noScale();

private:
	QString toGenerator;
	QTextStream toGen;


	QRectF bounds;
	int first_size_x;
	int first_size_y;
	int current_size_x;
	int current_size_y;
	int mStartX;
	int mStartY;
	int i;
	int j;
	int sep;
	QPainter *painter;
	QPen pen;
	QBrush brush;
	QString s1;
	QString s2;
	QFont font;
	QDomDocument doc;

	/** @brief false, если не надо масштабировать с учётом абсолютного задания
	*	координат, полезно при отрисовке иконок. True по умолчанию.
	**/
	bool mNeedScale;

	void line(QDomElement &element);
	void ellipse(QDomElement &element);
	void parsestyle(QDomElement &element);
	void background(QDomElement &element);
	void draw_text(QDomElement &element);
	void rectangle(QDomElement &element);
	void polygon(QDomElement &element);
	QPoint *getpoints(QDomElement &element, int n);
	void point (QDomElement &element);
	void defaultstyle();
	void path_draw(QDomElement &element);

	/** @brief Не знаю, что оно делает, но не могу смотреть на копипаст :)*/
	bool while_condition(QString str, int i);
};

class SdfIconEngineV2: public QIconEngine
{
public:
	SdfIconEngineV2(QString const &file);
	virtual void paint(QPainter *painter, QRect const &rect, QIcon::Mode mode, QIcon::State state);
private:
	SdfRenderer mRenderer;
};
