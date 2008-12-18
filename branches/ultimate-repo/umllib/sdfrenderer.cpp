#include "sdfrenderer.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFont>
#include <QIcon>
#include <QLineF>
#include <QTime>

SdfRenderer::SdfRenderer()
{
}

SdfRenderer::SdfRenderer(const QString path)
{
	load(path);
}

SdfRenderer::~SdfRenderer()
{
}

bool SdfRenderer::load( const QString & filename )
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		return false;
	}

	if (!doc.setContent(&file)) {
	    return false;
	}
	file.close();

	QDomElement docElem = doc.documentElement();
	first_size_x=docElem.attribute("sizex").toInt();
	first_size_y=docElem.attribute("sizey").toInt();

	return true;
}

void SdfRenderer::render(QPainter *painter, const QRectF & bounds)
{
	current_size_x = bounds.width();
	current_size_y = bounds.height();
	this->painter = painter;
	QDomElement docElem = doc.documentElement();
	QDomNode node = docElem.firstChild();
	while(!node.isNull())
	{
		QDomElement elem = node.toElement();
		if(!elem.isNull())
		{
			if (elem.tagName()=="line")
			{
				line(elem);
		    }
			else if(elem.tagName()=="ellipse")
			{
				ellipse(elem);
			}
			else if(elem.tagName()=="background")
			{
				background(elem);
			}
			else if(elem.tagName()=="text")
			{
				draw_text(elem);
			}
			else if (elem.tagName()=="rectangle")
			{
				rectangle(elem);
			}
			else if (elem.tagName()=="polygon")
			{
				polygon(elem);
			}
			else if (elem.tagName()=="point")
			{
				point(elem);
			}
			else if(elem.tagName()=="path")
			{
				path_draw(elem);
			}
		}
		node = node.nextSibling();
	}
	this->painter = 0;
}
void SdfRenderer::line(QDomElement &element)
{
	/*QFile log("log.txt");
	log.open(QIODevice::ReadWrite|QIODevice::Text);
	QTextStream to;
	to.setDevice(&log);*/
	//QMessageBox::information( 0,tr( "Critical Error"),tr( "Try to draw line" ));;

	float x1 = x1_def(element);
	float y1 = y1_def(element);
	float x2 = x2_def(element);
	float y2 = y2_def(element);
	QLineF line (x1,y1,x2,y2);


	parsestyle(element);
	painter->drawLine(line);
}
void SdfRenderer::ellipse(QDomElement &element)
{

	float x1 = x1_def(element);
	float y1 = y1_def(element);
	float x2 = x2_def(element);
	float y2 = y2_def(element);

	QRectF rect;
	rect.adjust(x1,y1,x2,y2);
	parsestyle(element);
	painter->drawEllipse(rect);
}

void SdfRenderer::background(QDomElement &element)
{
	parsestyle(element);
	painter->setPen(brush.color());
	painter->drawRect(painter->window());
	defaultstyle();
}
void SdfRenderer::draw_text(QDomElement &element)
{
	parsestyle(element);
	pen.setStyle(Qt::SolidLine);
	painter->setPen(pen);
	float x1 = x1_def(element);
	float y1 = y1_def(element);
	//int x1 = element.attribute("x1").toInt()*current_size_x/first_size_x;
	//int y1 = element.attribute("y1").toInt()*current_size_y/first_size_y;//+painter->font().pixelSize();
	QString str = element.text();
	// delete "\n" from the beginning of the string
	if (str[0]==13) str.remove(0,2);
	// delete "\n" from the end of the string
	if (str[str.length()-3]==13) str.remove(str.length()-3,2);

	while (str.contains(13))
	{
		int i = str.indexOf(13);
		QString temp = str.left(i);
		str.remove(0,i+2);
		painter->drawText(x1,y1, temp);
		y1+=painter->font().pixelSize() ;
	}
	QPointF point(x1,y1);
	painter->drawText(point, str);
	defaultstyle();
}
void SdfRenderer::rectangle(QDomElement &element)
{
	float x1 = x1_def(element);
	float y1 = y1_def(element);
	float x2 = x2_def(element);
	float y2 = y2_def(element);

	QRectF rect;
	rect.adjust(x1,y1,x2,y2);
	parsestyle(element);
	painter->drawRect(rect);
	defaultstyle();
}
void SdfRenderer::polygon(QDomElement &element)
{
	parsestyle(element);
	//QDomNode node = element.firstChild();
	QPoint *points = NULL;
	int n = element.attribute("n").toInt();
	if (!element.isNull())
	{
		points = getpoints(element,n);
	}
	if (points != NULL)
	{
		//painter->drawConvexPolygon(points, n);
		painter->drawConvexPolygon(points, n);
	}
	defaultstyle();
}
void SdfRenderer::point (QDomElement &element)
{
	parsestyle(element);
	float x = x1_def(element);
	float y = y1_def(element);
	QPointF pointf(x,y);
	painter->drawPoint(pointf);
	defaultstyle();
}
QPoint *SdfRenderer::getpoints(QDomElement &element, int n)
{
	QPoint *array = new QPoint[n];
	float x = 0;
	float y = 0;
	for (int i = 0; i < n; i++)
	{	QString str;
		str.setNum(i+1);
		QDomElement elem = element;
		QString xnum = elem.attribute(QString("x").append(str));
		if (xnum.endsWith("%"))
		{
			xnum.chop(1);
			x = current_size_x*xnum.toFloat()/100;
		}
		else if (xnum.endsWith("a"))
		{
			xnum.chop(1);
			x = xnum.toFloat();
		}
		else x = xnum.toFloat()*current_size_x/first_size_x;

		QString ynum = elem.attribute(QString("y").append(str));
		if (ynum.endsWith("%"))
		{
			ynum.chop(1);
			y = current_size_y*ynum.toFloat()/100;
		}
		else if (ynum.endsWith("a"))
		{
			ynum.chop(1);
			y = ynum.toFloat();
		}
		else y = ynum.toFloat()*current_size_y/first_size_y;

		array[i].setX(x);
		array[i].setY(y);
	}
	return array;

}
void SdfRenderer::defaultstyle()
{
	pen.setColor(QColor(0,0,0));
	brush.setColor(QColor(255,255,255));
	pen.setStyle(Qt::SolidLine);
	brush.setStyle(Qt::NoBrush);
	pen.setWidth(1);

}
void SdfRenderer::path_draw(QDomElement &element)
{
	/*QPointF end_point;
	QPointF c1;
	QPointF c2;
	bool fr=false;
	QDomNode node = element.firstChild();
	QPainterPath path;
	while(!node.isNull())
	{
		QDomElement elem = node.toElement();
		if(!elem.isNull())
		{
			if(elem.tagName()=="move")
			{
				end_point.setX(elem.attribute("x").toFloat()*current_size_x/first_size_x);
				end_point.setY(elem.attribute("y").toFloat()*current_size_y/first_size_y);
				path.moveTo(end_point);
			}
			if(elem.tagName()=="line")
			{
				end_point.setX(elem.attribute("x").toFloat()*current_size_x/first_size_x);
				end_point.setY(elem.attribute("y").toFloat()*current_size_y/first_size_y);
				path.lineTo(end_point);
			}
			if(elem.tagName()=="curve")
			{
				c1.setX(elem.attribute("x").toFloat()*current_size_x/first_size_x);
				c1.setY(elem.attribute("y").toFloat()*current_size_y/first_size_y);
				fr=false;
			}
			if(elem.tagName()=="curve_data")
			{
				if(!fr)
				{
					c2.setX(elem.attribute("x").toFloat()*current_size_x/first_size_x);
					c2.setY(elem.attribute("y").toFloat()*current_size_y/first_size_y);
					fr=true;
				}
				else
				{
					end_point.setX(elem.attribute("x").toFloat()*current_size_x/first_size_x);
					end_point.setY(elem.attribute("y").toFloat()*current_size_y/first_size_y);
					path.cubicTo(c1,c2,end_point);
				}
			}
		}
		node = node.nextSibling();
	}
	parsestyle(element);
	painter->drawPath(path);*/

	/*QPointF end_point;
	QPointF c1;
	QPointF c2;
	bool fr=false;
	QDomElement elem = element;
	QPainterPath path;
	//file.setFileName("Logger.txt");
	//file.open(QIODevice::ReadWrite|QIODevice::Text);
	//textstream.setDevice(&file);

	if (!elem.isNull())
	{
			QString d_cont;
			d_cont = elem.attribute("d").remove(0,1);

			sep = 1;
			for ( i = 0; i<d_cont.length();)
			{
					 if (d_cont.data()[i] == 'M')
					 {


						QString s = d_cont.section(' ', sep,sep+1);
						end_point.setX(d_cont.section(' ',sep,sep).toFloat()*current_size_x/first_size_x);
						end_point.setY(d_cont.section(' ',sep+1,sep+1).toFloat()*current_size_y/first_size_y);
						path.moveTo(end_point);
						 sep = sep +3;


					 }
					 else if (d_cont.data()[i] == 'L')
					 {
						QString s = d_cont.section(' ', sep,sep+1);
						end_point.setX(d_cont.section(' ',sep,sep).toFloat()*current_size_x/first_size_x);
						end_point.setY(d_cont.section(' ',sep+1,sep+1).toFloat()*current_size_y/first_size_y);
						path.lineTo(end_point);
						i = i+s.length()+3;
						 sep = sep+3;
					}
					 else if (d_cont.data()[i] == 'C')
					 {

						QString s = d_cont.section(' ', sep,sep+5);

						c1.setX(d_cont.section(' ', sep+2,sep+2).toFloat()*current_size_x/first_size_x);
						c1.setY(d_cont.section(' ', sep+3,sep+3).toFloat()*current_size_y/first_size_y);
						c2.setX(d_cont.section(' ', sep+4,sep+4).toFloat()*current_size_x/first_size_x);
						c2.setY(d_cont.section(' ', sep+5,sep+5).toFloat()*current_size_y/first_size_y);
						end_point.setX(d_cont.section(' ', sep,sep).toFloat()*current_size_x/first_size_x);
						end_point.setY(d_cont.section(' ', sep+1,sep+1).toFloat()*current_size_y/first_size_y);
						path.cubicTo(c1,c2,end_point);
						i =i+ s.length()+3;
						 sep = sep + 7;


					}


			}


		}

		parsestyle(element);
		painter->drawPath(path);*/

	QPointF end_point;
		QPointF c1;
		QPointF c2;
		bool fr=false;
		QDomElement elem = element;
		QPainterPath path;
		//file.setFileName("Logger.txt");
		//file.open(QIODevice::ReadWrite|QIODevice::Text);
		//textstream.setDevice(&file);

		if (!elem.isNull())
		{
			QString d_cont;
			d_cont = elem.attribute("d").remove(0,1);
			d_cont.append(" Z");

			for (i = 0; i<d_cont.length()-1;)
			{
				if (d_cont[i] == 'M')
				{
					j = i+2;
					while((!(d_cont[j] == 'L'))&&(!(d_cont[j] == 'C'))&&(!(d_cont[j] == 'M'))
							&&(!(d_cont[j] == 'Z'))&& (j != d_cont.length()))
					{

						while (!(d_cont[j] == ' '))
						{
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerM.txt", str);
							logger ("loggerM.txt", s1);*/
						}

						end_point.setX(s1.toFloat()*current_size_x/first_size_x);
						s1.clear();
						j = j+1;

						while (!(d_cont[j] == ' '))
						{
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerM.txt", str);
							logger ("loggerM.txt", s1);*/

						}

						end_point.setY(s1.toFloat()*current_size_y/first_size_y);
						j = j+1;
						s1.clear();

						/*QString str2;
						str2 = d_cont[j];
						logger ("loggerM.txt", str2);*/

					}

					path.moveTo(end_point);
					i = j;
				}

				else if (d_cont[i] == 'L')
				{
					j = i+2;
					while((!(d_cont[j] == 'M'))&&(!(d_cont[j] == 'C'))&&(!(d_cont[j] == 'L'))
							&&(!(d_cont[j] == 'Z'))&& (j != d_cont.length()))
					{
						while (!(d_cont[j] == ' '))
						{
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerL.txt", str);
							logger ("loggerL.txt", s1);*/
						}

						end_point.setX(s1.toFloat()*current_size_x/first_size_x);
						s1.clear();
						j = j+1;

						while (!(d_cont[j] == ' '))
						{
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerL.txt", str);
							logger ("loggerL.txt", s1);*/
						}
						end_point.setY(s1.toFloat()*current_size_y/first_size_y);
						j = j+1;
						s1.clear();


						/*QString str2;
						str2 = d_cont[j];
						logger("loggerL.txt", str2);*/

					}

					 path.lineTo(end_point);
					 i = j;
				}

				 else if (d_cont[i] == 'C')
			 	{
				 	j = i+2;
					while((!(d_cont[j] == 'M'))&&(!(d_cont[j] == 'L'))&&(!(d_cont[j] == 'C'))
							&&(!(d_cont[j] == 'Z'))&&(j != d_cont.length()))
					{
						while (!(d_cont[j] == ' '))
						{
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerC.txt", str);
							logger ("loggerC.txt", s1);*/
						}

						c1.setX(s1.toFloat()*current_size_x/first_size_x);
						s1.clear();
						j = j+1;

						while (!(d_cont[j] == ' '))
						{
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerC.txt", str);
							logger ("loggerC.txt", s1);*/
						}

						c1.setY(s1.toFloat()*current_size_y/first_size_y);
						s1.clear();
						j = j+1;

						while (!(d_cont[j] == ' '))
						{
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerC.txt", str);
							logger ("loggerC.txt", s1);*/

						}

						c2.setX(s1.toFloat()*current_size_x/first_size_x);
						s1.clear();
						j = j+1;

						while (!(d_cont[j] == ' '))
						{
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerC.txt", str);
							logger ("loggerC.txt", s1);*/
						}

						c2.setY(s1.toFloat()*current_size_y/first_size_y);
						s1.clear();
						j = j+1;

						 while (!(d_cont[j] == ' '))
						 {
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerC.txt", str);
							logger ("loggerC.txt", s1);*/
						 }

						 end_point.setX(s1.toFloat()*current_size_x/first_size_x);
						 s1.clear();
						 j = j+1;

						 while (!(d_cont[j] == ' '))
						 {
							s1.append(d_cont[j]);
							j = j+1;
							/*QString str;
							str.setNum(j);
							logger("loggerC.txt", str);
							logger ("loggerC.txt", s1);*/
						 }

						 end_point.setY(s1.toFloat()*current_size_y/first_size_y);
						 s1.clear();
						 j = j+1;

						/* QString str2;
						 str2 = d_cont[j];
						 logger("loggerC.txt", str2);*/

					}

					path.cubicTo(c1,c2,end_point);
					i =j;

				} else if (d_cont[i] == 'Z')
				{	//if (i = d_cont.l)
					path.closeSubpath();
					logger ("loggerZ.txt", "DONE");
				}


			}
	}



			parsestyle(element);

			painter->drawPath(path);
}


void SdfRenderer::parsestyle(QDomElement &element)
{
	QDomElement elem = element;
	if(!elem.isNull())
	{
		if (elem.hasAttribute("stroke-width"))
		{
			pen.setWidth(elem.attribute("stroke-width").toInt());
		}
		if (elem.hasAttribute("fill"))
		{
			QColor color = elem.attribute("fill");
			brush.setStyle(Qt::SolidPattern);
			brush.setColor(color);
		}
		if (elem.hasAttribute("stroke"))
		{
			QColor color = elem.attribute("stroke");
			pen.setColor(color);
		}

		if (elem.hasAttribute("stroke-style"))
		{
			if (elem.attribute("stroke-style") == "solid") pen.setStyle(Qt::SolidLine);
			if (elem.attribute("stroke-style") == "dot") pen.setStyle(Qt::DotLine);
			if (elem.attribute("stroke-style") == "dash") pen.setStyle(Qt::DashLine);
			if (elem.attribute("stroke-style") == "dashdot") pen.setStyle(Qt::DashDotLine);
			if (elem.attribute("stroke-style") == "dashdotdot") pen.setStyle(Qt::DashDotDotLine);
			if (elem.attribute("stroke-style") == "none") pen.setStyle(Qt::NoPen);
		}//  pen.setStyle(Qt::NoPen);

		if (elem.hasAttribute("fill-style"))
		{
			if (elem.attribute("fill-style")=="none") brush.setStyle(Qt::NoBrush);
			else if(elem.attribute("fill-style")=="solid") brush.setStyle(Qt::SolidPattern);
		}

		if (elem.hasAttribute("font-fill"))
		{
			QColor color = elem.attribute("font-fill");
			pen.setColor(color);
		}
		if (elem.hasAttribute("font-size"))
		{
			QString fontsize = elem.attribute("font-size");
			if (fontsize.endsWith("%"))
			{
				fontsize.chop(1);
				font.setPixelSize(current_size_y*fontsize.toInt()/100);
			}
			else if (fontsize.endsWith("a"))
			{
				fontsize.chop(1);
				font.setPixelSize(fontsize.toInt());
			}
			else font.setPixelSize(fontsize.toInt()*current_size_y/first_size_y);
		}
		if (elem.hasAttribute("font-name"))
		{
			font.setFamily(elem.attribute("font-name"));
		}
		if (elem.hasAttribute("b"))
		{
			font.setBold(elem.attribute("b").toInt());
		}
		if (elem.hasAttribute("i"))
		{
			font.setItalic(elem.attribute("i").toInt());
		}
		if (elem.hasAttribute("u"))
		{
			font.setUnderline(elem.attribute("u").toInt());
		}

		painter->setFont(font);
		}




		 painter->setPen(pen);
		 painter->setBrush(brush);


}
float SdfRenderer::x1_def(QDomElement &element)
{
	float x1 = 0;
	QString x1str = element.attribute("x1");

	if (x1str.endsWith("%"))
	{
		x1str.chop(1);
		x1 = current_size_x*x1str.toFloat()/100;
		return x1;
	}
	else if (x1str.endsWith("a"))
	{
		x1str.chop(1);
		x1 = x1str.toFloat();
		return x1;
	}
	else
	{
	x1 = x1str.toFloat()*current_size_x/first_size_x;
	return x1;
	}
}
float SdfRenderer::y1_def(QDomElement &element)
{
	float y1 = 0;
	QString y1str = element.attribute("y1");

	if (y1str.endsWith("%"))
	{
		y1str.chop(1);
		y1 = current_size_y*y1str.toFloat()/100;
		return y1;
	}
	else if (y1str.endsWith("a"))
	{
		y1str.chop(1);
		y1 = y1str.toFloat();
		return y1;
	}
	else
	{
	y1 = y1str.toFloat()*current_size_y/first_size_y;
	return y1;
	}
}
float SdfRenderer::x2_def(QDomElement &element)
{
	float x2 = 0;
	QString x2str = element.attribute("x2");

	if (x2str.endsWith("%"))
	{
		x2str.chop(1);
		x2 = current_size_x*x2str.toFloat()/100;
		return x2;
	}
	else if (x2str.endsWith("a"))
	{
		x2str.chop(1);
		x2 = x2str.toFloat();
		return x2;
	}
	else
	{
	x2 = x2str.toFloat()*current_size_x/first_size_x;
	return x2;
	}
}
float SdfRenderer::y2_def(QDomElement &element)
{
	float y2 = 0;
	QString y2str = element.attribute("y2");

	if (y2str.endsWith("%"))
	{
		y2str.chop(1);
		y2 = current_size_y*y2str.toFloat()/100;
		return y2;
	}
	else if (y2str.endsWith("a"))
	{
		y2str.chop(1);
		y2 = y2str.toFloat();
		return y2;
	}
	else
	{
	y2 = y2str.toFloat()*current_size_y/first_size_y;
	return y2;
	}
}
void SdfRenderer::logger(QString path, QString string)
{	log.setFileName(path);
	log.open(QIODevice::Append|QIODevice::Text);
	logtext.setDevice(&log);
	logtext<<string<<"\n";
	log.close();

}

