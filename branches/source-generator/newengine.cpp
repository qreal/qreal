#include "newengine.h"
#include <QFile>
#include <QBrush>
#include <QPainter>
#include <QVector>
#include <QPolygon>
#include <QPainterPath>
#include <QMatrix>

NewEngine::NewEngine(QString path,QPoint size, QPaintEngine *parent)
{
	outfile.setFileName(path);
	outfile.open(QIODevice::WriteOnly|QIODevice::Truncate | QIODevice::Text);
	textstream.setDevice(&outfile);
	textstream<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?> \n\n";
	textstream<<"<picture sizex = \""<<size.x()<<"\" sizey = \""<<size.y()<<"\">\n\n";





	//textstream.seek(outfile.size());
}

/*bool NewEngine::hasFeature ( PaintEngineFeatures feature ) const		// -
{
	return true;
}*/

void NewEngine::drawLines ( const QLine * lines, int lineCount )
{
//	textstream<<__PRETTY_FUNCTION__;
	for (int i=0; i<lineCount; i++)
	{
		textstream<<"\t<line x1 = \""<<lines[i].x1()<<"\" y1 = \""<<lines[i].y1()<<"\" ";
		textstream<<"x2 = \""<<lines[i].x2()<<"\" y2 = \""<<lines[i].y2()<<"\" \n";
		stylePrint();
		textstream<<"/>\n\n";
		//textstream<<"\t</line>\n\n";
	};
	//textstream.flush();
}
void NewEngine::drawLines ( const QLineF * lines, int lineCount )
{
//	textstream<<__PRETTY_FUNCTION__;
	for (int i=0; i<lineCount; i++)
	{
		/*textstream<<"\t<line x1 = \""<<lines[i].x1()<<"\" y1 = \""<<lines[i].y1()<<"\" ";
		textstream<<"x2 = \""<<lines[i].x2()<<"\" y2 = \""<<lines[i].y2()<<"\">\n";
		stylePrint();
		textstream<<"\t</line>\n\n";*/
		textstream<<"\t<line x1 = \""<<lines[i].x1()<<"\" y1 = \""<<lines[i].y1()<<"\" ";
		textstream<<"x2 = \""<<lines[i].x2()<<"\" y2 = \""<<lines[i].y2()<<"\" \n";
		stylePrint();
		textstream<<"/>\n\n";
	};
	//textstream.flush();
}
void NewEngine::drawPolygon ( const QPoint * points, int pointCount, PolygonDrawMode mode )
{
//	textstream<<__PRETTY_FUNCTION__;
	textstream<<"\t<polygon n = \""<<pointCount<<"\">\n";
	textstream<<"\t\t<vertices>\n";
	for (int i=0; i<pointCount; i++)
	{
		textstream<<"\t\t\t<point num = \""<<i+1<<"\" ";
		textstream<<"x = \""<<points[i].x()<<"\" ";
		textstream<<"y = \""<<points[i].y()<<"\"/>\n";
	};
	textstream<<"\t\t</vertices>\n";
	stylePrint();
	textstream<<"</polygon>\n";
}

void NewEngine::drawPolygon ( const QPointF * points, int pointCount, PolygonDrawMode mode )
{
//	textstream<<__PRETTY_FUNCTION__;
	textstream<<"\t<polygon n = \""<<pointCount<<"\"\n";
	for (int i=0; i<pointCount; i++)
	{
		QString str;
		str.setNum(i+1);
		//textstream<<"\t\t\t<point num = \""<<i+1<<"\" ";

		textstream<<"\t x"<<str<<" = \""<<int(points[i].x())<<"\" ";
		textstream<<"y"<<str<<" = \""<<int(points[i].y())<<"\"\n";
	};
	stylePrint();
	textstream<<"/>\n";
//	textstream.flush();
}

void NewEngine::drawEllipse ( const QRect & rect )
{
//	textstream<<__PRETTY_FUNCTION__;
	textstream<<"\t<ellipse x1 = \""<<rect.x()<<"\" y1 = \""<<rect.y()<<"\" ";
	textstream<<"x2 = \""<<rect.width()+ rect.x()<<"\" y2 = \""<<rect.height()+rect.y()<<"\" \n";
	stylePrint();
	textstream<<"/>\n\n";
}

void NewEngine::drawEllipse ( const QRectF & rect )
{
//	textstream<<__PRETTY_FUNCTION__;
	textstream<<"\t<ellipse x1 = \""<<rect.x()<<"\" y1 = \""<<rect.y()<<"\" ";
		textstream<<"x2 = \""<<rect.width()+ rect.x()<<"\" y2 = \""<<rect.height()+rect.y()<<"\" \n";
		stylePrint();
		textstream<<"/>\n\n";
}
void NewEngine::drawPoints ( const QPoint * points, int pointCount )
{
//	textstream<<__PRETTY_FUNCTION__;
	for (int i=0;i<pointCount;i++)
	{
		textstream<<"\t<point x1 = \""<<points[i].x()<<"\" y1 = \""<<points[i].y()<<"\" \n";
		stylePrint();
		textstream<<"/>\n\n";
	}
	textstream.flush();
}

void NewEngine::drawPoints ( const QPointF * points, int pointCount )
{
//	textstream<<__PRETTY_FUNCTION__;
	for (int i=0;i<pointCount;i++)
	{
		textstream<<"\t<point x1 = \""<<points[i].x()<<"\" y1 = \""<<points[i].y()<<"\" \n";
		stylePrint();
		textstream<<"/>\n\n";
	}
	textstream.flush();
}

void NewEngine::drawRects ( const QRectF * rects, int rectCount )
{
//	textstream<<__PRETTY_FUNCTION__;
	for (int i=0; i<rectCount; i++)
	{
		textstream<<"\t<rectangle x1 = \""<<rects[i].x()<<"\" ";
		textstream<<"y1 = \""<<rects[i].y()<<"\" ";
		textstream<<"x2 = \""<<rects[i].width()+rects[i].x()<<"\" ";
		textstream<<"y2 = \""<<rects[i].height()+rects[i].y()<<"\" \n";
		stylePrint();
		textstream<<"/>\n\n";
	}

}

void NewEngine::drawRects ( const QRect * rects, int rectCount )
{
//	textstream<<__PRETTY_FUNCTION__;
	for (int i=0; i<rectCount; i++)
	{
		textstream<<"\t<rectangle x1 = \""<<rects[i].x()<<"\" ";
		textstream<<"y1 = \""<<rects[i].y()<<"\" ";
		textstream<<"x2 = \""<<rects[i].width()+rects[i].x()<<"\" ";
		textstream<<"y2 = \""<<rects[i].height()+rects[i].y()<<"\" \n";
		stylePrint();
		textstream<<"/>\n\n";
	}

}
void NewEngine::drawPath ( const QPainterPath & path )
{
//	textstream<<__PRETTY_FUNCTION__;

	int count = path.elementCount();
	QPainterPath::Element elem;
	textstream<<"\t<path int = \""<<count<<"\" ";
	textstream<<"d = \"";
	//stylePrint();
	//textstream<<">\n";

	for(int i=0;i<count;i++)
	{
		elem = path.elementAt(i);
		if (elem.type==QPainterPath::LineToElement)
		{
			textstream<<" L "<<float(elem.x)<<" ";
			textstream<<float(elem.y);
		}
		else if(elem.type==QPainterPath::CurveToElement)
		{
			textstream<<" C "<<float(elem.x)<<" ";
			textstream<<float(elem.y);

		}else if(elem.type==QPainterPath::MoveToElement)
		{
			textstream<<" M "<<float(elem.x)<<" ";
			textstream<<float(elem.y);

		}else if(elem.type==QPainterPath::CurveToDataElement)
		{
			textstream<<" "<<float(elem.x)<<" ";
			textstream<<float(elem.y);
		}
	}
	textstream<<"\"";
	stylePrint();
	textstream<<"/>\n\n";
}
void NewEngine::drawPixmap ( const QRectF & r, const QPixmap & pm, const QRectF & sr )
{
//	textstream<<__PRETTY_FUNCTION__;
	textstream<<"DrawPixmap\n";
	textstream.flush();
}
void NewEngine::drawTextItem ( const QPointF & p, const QTextItem & textItem )
{
//	textstream<<__PRETTY_FUNCTION__;
	textstream<<"\t<text x1 = \""<<int(p.x())<<"\" ";
	textstream<<"y1 = \""<<int(p.y())<<"\"\n";
	textstream<<"\t\tfont-fill = \""<<QString("#%1").arg(painter()->pen().color().rgba(),0, 16).remove(1,2)<<"\" ";
	textstream<<"font-size = \""<<textItem.font().pixelSize()<<"\" ";
	textstream<<"font-name = \""<<textItem.font().family()<<"\" ";
	textstream<<"b = \""<<textItem.font().bold()<<"\" ";
	textstream<<"i = \""<<textItem.font().italic()<<"\" ";
	textstream<<"u = \""<<textItem.font().underline()<<"\"> \n";
	textstream<<textItem.text()<<"\n";
	textstream<<"\t</text>\n\n";

	textstream.flush();
}
void NewEngine::drawTiledPixmap ( const QRectF & rect, const QPixmap & pixmap, const QPointF & p )
{
//	textstream<<__PRETTY_FUNCTION__;
	textstream<<"DrawTiledPixmap\n";
	textstream.flush();
}
void NewEngine::drawImage ( const QRectF & rect, const QImage & image, const QRectF & sr,Qt::ImageConversionFlags flags )
{
//	textstream<<__PRETTY_FUNCTION__;
	textstream<<"DrawImage\n";
	textstream<<rect.x()<<" "<<rect.y()<<" "<<rect.width()<<" "<<rect.height()<<"\n";
	stylePrint();
	textstream<<"\n";
	textstream.flush();
}

void NewEngine::stylePrint()
{
//	textstream<<__PRETTY_FUNCTION__;

	textstream<<"\t stroke = \""<<QString("#%1").arg(painter()->pen().color().rgba(),0, 16).remove(1,2)<<"\" ";
	textstream<<"stroke-width = \""<<painter()->pen().width()<<"\" ";
	textstream<<"stroke-style = \""<<styletostring_pen(painter()->pen().style())<<"\" ";
	textstream<<"fill = \""<<QString("#%1").arg(painter()->brush().color().rgba(),0, 16).remove(1,2)<<"\" ";
	textstream<<"fill-style = \""<<styletostring_brush(painter()->brush().style())<<"\"";


}
QString NewEngine::styletostring_pen(int style)
{

	if (style = Qt::SolidLine) return "solid";
	if (style = Qt::DotLine) return "dot";
	if (style = Qt::DashLine) return "dash";
	if (style = Qt::DashDotLine) return "dashdot";
	if (style = Qt::DashDotDotLine) return "dashdotdot";
	if (style = Qt::NoPen) return "none";
}

QString NewEngine::styletostring_brush(int style)
{
	if (style = Qt::SolidPattern) return "solid";
	if (style = Qt::NoBrush) return "none";

}
NewEngine::~NewEngine()
{	textstream<<"</picture>";
	textstream.flush();
	outfile.close();
}
bool NewEngine::hasFeature ( PaintEngineFeatures feature ) const
{
	return true;
}

/*void NewEngine::type_matrix()
{
	textstream<<"\t\t<matrix>\n";
	textstream<<"\t\t\t<components m11 = \""<<this->painter()-> transform().m11()<<"\" ";
	textstream<<"m21 = \""<<painter()-> transform().m21()<<"\" ";
	textstream<<"m12 = \""<<painter()-> transform().m12()<<"\" ";
	textstream<<"m22 = \""<<painter()-> transform().m22()<<"\" ";
	textstream<<"dx = \""<<painter()->transform().dx()<<"\" ";
	textstream<<"dy = \""<<painter()->transform().dy()<<"\"/>\n";
	textstream<<"\t\t</matrix>\n";


}*/
