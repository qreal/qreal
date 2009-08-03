#pragma once

#include <QPaintEngine>
#include <QFile>
#include <QTextStream>
#include <QPoint>

namespace sdfExtractor
{

	class NewEngine: public QPaintEngine
	{
	public:
		NewEngine(QString path, QPoint size, QPaintEngine *parent = NULL);
		~NewEngine();

		void drawLines(const QLine *lines, int lineCount);
		bool begin(QPaintDevice *pdev) { return true; }
		void drawEllipse(const QRectF &rect);
		void drawEllipse(const QRect &rect);
		void drawImage(const QRectF &rectangle, const QImage &image,
					   const QRectF &sr, Qt::ImageConversionFlags flags);
		void drawLines(const QLineF *lines, int lineCount);
		void drawPath(const QPainterPath &path);
		void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
		void drawPoints(const QPointF *points, int pointCount);
		void drawPoints(const QPoint *points, int pointCount);
		void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
		void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode);
		void drawRects(const QRectF *rects, int rectCount);
		void drawRects(const QRect *rects, int rectCount);
		void drawTextItem(const QPointF &p, const QTextItem & textItem);
		void drawTiledPixmap(const QRectF &rect, const QPixmap &pixmap, const QPointF &p);
		void updateState(const QPaintEngineState &state) {}
		bool hasFeature(PaintEngineFeatures feature) const;
		bool end() { return true; }
//		void drawScrollArea (const QRectF *rects);
		Type type() const { return SVG; }
	private:
		void stylePrint();
		QString styletostring_pen(int style);
		QString styletostring_brush(int style);
		QFile outfile;
		QTextStream textstream;
	};

}
