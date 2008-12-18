#ifndef SDFRENDERER_H
#define SDFRENDERER_H

#include <QtGui/QWidget>
#include <QtXml/QDomDocument>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QFont>
#include <QFile>
#include <QTextStream>

class SdfRenderer : public QObject
{
    Q_OBJECT

public:
	SdfRenderer();
	SdfRenderer(const QString path);
        ~SdfRenderer();

	bool load (const QString &filename);
	void render(QPainter *painter, const QRectF &bounds);

private:
	int first_size_x;
	int first_size_y;
	int current_size_x;
	int current_size_y;
	int i;
	int j;
	int sep;
	QPainter *painter;
	QPen pen;
	QBrush brush;
	QString s1;
	QString s2;
	QFont font;
	QFile log;
	QTextStream logtext;
        QDomDocument doc;
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
	float x1_def(QDomElement &element);
	float y1_def(QDomElement &element);
	float x2_def(QDomElement &element);
	float y2_def(QDomElement &element);
	void logger(QString path,QString string);
};

#endif // SDFRENDERER
