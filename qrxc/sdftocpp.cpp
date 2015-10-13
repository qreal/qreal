/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "sdftocpp.h"

#include <QtWidgets/QMessageBox>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtCore/QLineF>
#include <QtCore/QTime>
#include <QtCore/QDebug>

SdfRenderer::SdfRenderer()
	: mStartX(0), mStartY(0), mNeedScale(true)
{
	toGen.setString(&toGenerator,QIODevice::ReadWrite);
}

SdfRenderer::SdfRenderer(const QString path)
	: mStartX(0), mStartY(0), mNeedScale(true)
{
	if (!load(path))
	{
		qDebug() << "File " + path + " - loading failed!";
	}
	toGen.setString(&toGenerator,QIODevice::ReadWrite);
}

SdfRenderer::~SdfRenderer()
{
}

SdfRenderer::SdfRenderer(const QDomNode &bla)
{
	toGen.setString(&toGenerator, QIODevice::ReadWrite);
	QTextStream str;
	QString tr;
	str.setString(&tr,QIODevice::ReadWrite);
	str << bla;
	doc.setContent(*(str.string()));
	QDomElement docElem = doc.documentElement();
	first_size_x = docElem.attribute("sizex").toInt();
	first_size_y = docElem.attribute("sizey").toInt();
}

bool SdfRenderer::load(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();

	QDomElement docElem = doc.documentElement();
	first_size_x = docElem.attribute("sizex").toInt();
	first_size_y = docElem.attribute("sizey").toInt();

	return true;
}

QString SdfRenderer::render()
{
	current_size_x = first_size_x;
	current_size_y = first_size_y;
	mStartX = static_cast<int>(bounds.x());
	mStartY = static_cast<int>(bounds.y());
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
	return toGenerator;
}

void SdfRenderer::line(QDomElement &element)
{
	toGen << "{\n";
	toGen << QString("\tfloat x1 = x_def(\"%1\");\n").arg(element.attribute("x1"));
	toGen << QString("\tfloat y1 = y_def(\"%1\");\n").arg(element.attribute("y1"));
	toGen << QString("\tfloat x2 = x_def(\"%1\");\n").arg(element.attribute("x2"));
	toGen << QString("\tfloat y2 = y_def(\"%1\");\n").arg(element.attribute("y2"));
	toGen <<"\tQLineF line (x1,y1,x2,y2);\n\n";
	parsestyle(element);
	toGen <<"\tpainter->drawLine(line);\n";
	toGen << "};\n\n";

}

void SdfRenderer::ellipse(QDomElement &element)
{
	toGen << "{\n";
	toGen << QString("\tfloat x1 = x_def(\"%1\");\n").arg(element.attribute("x1"));
	toGen << QString("\tfloat y1 = y_def(\"%1\");\n").arg(element.attribute("y1"));
	toGen << QString("\tfloat x2 = x_def(\"%1\");\n").arg(element.attribute("x2"));
	toGen << QString("\tfloat y2 = y_def(\"%1\");\n").arg(element.attribute("y2"));
	toGen << "\tQRectF rect;\n";
	toGen << "\trect.adjust(x1, y1, x2, y2);\n";
	parsestyle(element);
	toGen << "\tpainter->drawEllipse(rect);\n";
	toGen << "};\n\n";
}

void SdfRenderer::background(QDomElement &element)//FIXME:
{
	parsestyle(element);
	toGen << "\tpainter->setPen(brush.color());\n";
	toGen << "\tpainter->drawRect(painter->window());\n";
}

void SdfRenderer::draw_text(QDomElement &element)
{
	toGen << "{\n";
	//parsestyle(element);
	toGen << "\tQPen pen;\n";
	toGen << "\tQFont font;\n";
	toGen << "\tpen.setStyle(Qt::SolidLine);\n";
	if (element.hasAttribute("font-fill"))
	{
		QColor color = element.attribute("font-fill");
		int red = color.red();
		int green = color.green();
		int blue = color.blue();
		toGen << "\tpen.setColor(QColor("<< red << ", "<< green << ", " << blue <<"));\n";
	}
	if (element.hasAttribute("font-size"))
	{
		QString fontsize = element.attribute("font-size");
		if (fontsize.endsWith("%"))
		{
			fontsize.chop(1);
			toGen << "\tfont.setPixelSize(y_def(\"" << fontsize.toInt() << "\"));\n";
		}
		else if (fontsize.endsWith("a") && mNeedScale)
		{
			fontsize.chop(1);
			toGen << "\tfont.setPixelSize(" << fontsize.toInt() << ");\n";
		}
		else if (fontsize.endsWith("a") && !mNeedScale)
		{
			fontsize.chop(1);
			toGen << "\tfont.setPixelSize(" << fontsize.toInt() << ");\n";
		}
		else
			toGen << "\tfont.setPixelSize(y_def(\"" << fontsize.toInt() << "\"));\n";
		}

		if (element.hasAttribute("font-name"))
		{
			toGen << "\tfont.setFamily(\"" << element.attribute("font-name") << "\");\n";
		}

		if (element.hasAttribute("b"))
		{
			toGen << "\tfont.setBold(" << element.attribute("b").toInt() << ");\n";
		}

		if (element.hasAttribute("i"))
		{
			toGen << "\tfont.setItalic(" << element.attribute("i").toInt() << ");\n";
		}

		if (element.hasAttribute("u"))
		{
			toGen << "\tfont.setUnderline(" << element.attribute("u").toInt() << ");\n";
		}
	toGen << QString("\n\tfloat x1 = x_def(\"%1\");\n").arg(element.attribute("x1"));
	toGen << QString("\tfloat y1 = y_def(\"%1\");\n\n").arg(element.attribute("y1"));
	QString str = element.text();

	// delete "\n" from the beginning of the string
	if (str[0] == '\n')
		str.remove(0, 1);

	// delete "\n" from the end of the string
	if (str[str.length() - 1] == '\n')
		str.remove(str.length() - 1, 1);

	while (str.contains('\n'))
	{
		int i = str.indexOf('\n');
		QString temp = str.left(i);
		str.remove(0, i + 1);
	}

	toGen << "\tQString str(\"" << str << "\");\n";
	toGen << "\tQPointF point(x1, y1);\n";
	toGen << "\tpainter->setPen(pen);\n";
	toGen << "\tpainter->setFont(font);\n";
	toGen << "\tpainter->drawText(point, str);\n";
	toGen << "};\n\n";
}

void SdfRenderer::rectangle(QDomElement &element)
{
	toGen << "{\n";
	toGen << QString("\tfloat x1 = x_def(\"%1\");\n").arg(element.attribute("x1"));
	toGen << QString("\tfloat y1 = y_def(\"%1\");\n").arg(element.attribute("y1"));
	toGen << QString("\tfloat x2 = x_def(\"%1\");\n").arg(element.attribute("x2"));
	toGen << QString("\tfloat y2 = y_def(\"%1\");\n").arg(element.attribute("y2"));
	toGen << "\tQRectF rect;\n";
	toGen << "\trect.adjust(x1, y1, x2, y2);\n";
	parsestyle(element);
	toGen << "\n\tpainter->drawRect(rect);\n";
	toGen << "};\n\n";
}

void SdfRenderer::polygon(QDomElement &element)
{
	parsestyle(element);
	// FIXME: init points array here
	QPoint *points = nullptr;
	int n = element.attribute("n").toInt();
	if (!element.isNull())
	{
		points = getpoints(element, n);
	}
	if (points != nullptr)
	{
//		painter->drawConvexPolygon(points, n);
		delete[] points;
	}
	defaultstyle();
}

void SdfRenderer::point(QDomElement &element)
{
	toGen << "{\n";
	toGen << QString("\tfloat x = x_def(\"%1\");\n").arg(element.attribute("x1"));
	toGen << QString("\tfloat y = y_def(\"%1\");\n").arg(element.attribute("y1"));
	parsestyle(element);
	toGen << "\tQPointF point(x,y);\n";
	toGen << "\tpainter->drawPoint(point);\n";
	toGen << "};\n\n";
}

QPoint *SdfRenderer::getpoints(QDomElement &element, int n)
{
	QPoint *array = new QPoint[n];
	float x = 0;
	float y = 0;
	for (int i = 0; i < n; i++)
	{
		QString str;
		str.setNum(i + 1);
		QDomElement elem = element;
		QString xnum = elem.attribute(QString("x").append(str));
		if (xnum.endsWith("%"))
		{
			xnum.chop(1);
			x = current_size_x * xnum.toFloat() / 100 + mStartX;
		}
		else if (xnum.endsWith("a") && mNeedScale)
		{
			xnum.chop(1);
			x = xnum.toFloat() + mStartX;
		}
		else if (xnum.endsWith("a") && !mNeedScale)
		{
			xnum.chop(1);
			x = xnum.toFloat() * current_size_x / first_size_x + mStartX;
		}
		else
			x = xnum.toFloat() * current_size_x / first_size_x + mStartX;

		QString ynum = elem.attribute(QString("y").append(str));
		if (ynum.endsWith("%"))
		{
			ynum.chop(1);
			y = current_size_y * ynum.toFloat() / 100 + mStartY;
		}
		else if (ynum.endsWith("a") && mNeedScale)
		{
			ynum.chop(1);
			y = ynum.toFloat() + mStartY;
		}
		else if (ynum.endsWith("a") && !mNeedScale)
		{
			ynum.chop(1);
			y = ynum.toFloat() * current_size_y / first_size_y + mStartY;
		}
		else
			y = ynum.toFloat() * current_size_y / first_size_y + mStartY;

		array[i].setX(static_cast<int>(x));
		array[i].setY(static_cast<int>(y));
	}
	return array;
}

void SdfRenderer::defaultstyle()
{
	/*toGen << QString("pen.setColor(QColor(0,0,0));\n");
	toGen << QString("brush.setColor(QColor(255,255,255));\n");
	toGen << QString("pen.setStyle(Qt::SolidLine);\n");
	toGen << QString("brush.setStyle(Qt::NoBrush);\n");
	toGen << QString("pen.setWidth(1);\n");
	*/
}

bool SdfRenderer::while_condition(QString str, int i)
{
	return (str[i] != 'L') && (str[i] != 'C') && (str[i] != 'M')
		&& (str[i] != 'Z') && (i != str.length());
}

void SdfRenderer::path_draw(QDomElement &element)
{
	QDomElement elem = element;
	toGen << "{\n";
	toGen << QString("\tQPainterPath path;\n");
	toGen << QString("\tQPointF end_point;\n");
	toGen << QString("\tQPointF c1;\n");
	toGen << QString("\tQPointF c2;\n\n");

	if (!elem.isNull())
	{
		QString d_cont;

		d_cont = elem.attribute("d").remove(0, 1);
		d_cont.append(" Z");

		for (i = 0; i < d_cont.length() - 1;)
		{
			if (d_cont[i] == 'M')
			{
				j = i + 2;
				while (while_condition(d_cont, j))
				{
					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}
					toGen << "end_point.setX(x_def(\"" << s1 << "\"));\n";
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					toGen << "end_point.setY(y_def(\"" << s1 << "\"));\n";
					++j;
					s1.clear();
				}
				toGen << QString("path.moveTo(end_point);\n\n");
				i = j;
			}
			else if (d_cont[i] == 'L')
			{
				j = i + 2;
				while (while_condition(d_cont, j))
				{
					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}
					toGen << "\tend_point.setX(x_def(\"" << s1 << "\"));\n";
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}
					toGen << "\tend_point.setY(y_def(\"" << s1 << "\"));\n";
					++j;
					s1.clear();
				}

				toGen << QString("\tpath.lineTo(end_point);\n\n");
				i = j;
			}
			 else if (d_cont[i] == 'C')
			{
				j = i + 2;
				while(while_condition(d_cont, j))
				{
					while (!(d_cont[j] == ' '))
					{
						s1.append(d_cont[j]);
						++j;
					}
					toGen << "\tc1.setX(x_def(\"" << s1 << "\"));\n";
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					toGen << "\tc1.setY(y_def(\"" << s1 << "\"));\n";
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					toGen << "\tc2.setX(x_def(\"" << s1 << "\"));\n";
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					toGen << "\tc2.setY(y_def(\"" << s1 << "\"));\n";
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					toGen << "\tend_point.setX(x_def(\"" << s1 << "\"));\n";
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					toGen << "\tend_point.setY(y_def(\"" << s1 << "\"));\n";
					s1.clear();
					++j;
				}
				toGen << QString("\tpath.cubicTo(c1, c2, end_point);\n\n");
				i = j;

			} else if (d_cont[i] == 'Z')
			{
				toGen << "path.closeSubpath();\n";
			}
		}
	}

	parsestyle(element);
	toGen << QString("\tpainter->drawPath(path);\n");
	toGen << "};\n\n";
}

void SdfRenderer::parsestyle(QDomElement &element)
{
	QDomElement elem = element;
	toGen << QString("\n\tQPen pen;\n");
	toGen << QString("\tQBrush brush;\n");
	toGen << QString("\tQFont font;\n\n");
	if(!elem.isNull())
	{
		if (elem.hasAttribute("stroke-width"))
		{
			if (mNeedScale)
				toGen << "\tpen.setWidth(" << elem.attribute("stroke-width").toInt() << ");\n";
				else  // Для отрисовки иконок - ширина всех линий устанавливается в 1.
				toGen << "\tpen.setWidth(1);\n";
		}

		if (elem.hasAttribute("fill"))
		{
			QColor color = elem.attribute("fill");
			int red = color.red();
			int green = color.green();
			int blue = color.blue();
			toGen << QString("\tbrush.setStyle(Qt::SolidPattern);\n");
			toGen << "\tbrush.setColor(QColor("<< red << ", "
				  << green << ", " << blue <<"));\n";
		}

		if (elem.hasAttribute("stroke"))
		{
			QColor color = elem.attribute("stroke");
			int red = color.red();
			int green = color.green();
			int blue = color.blue();
			toGen << "\tpen.setColor(QColor("<< red << ", "
				  << green << ", " << blue <<"));\n";
		}

		if (elem.hasAttribute("stroke-style"))
		{
			if (elem.attribute("stroke-style") == "solid")
				toGen << "\tpen.setStyle(Qt::SolidLine);\n";
			if (elem.attribute("stroke-style") == "dot")
				toGen << "\tpen.setStyle(Qt::DotLine);\n";
			if (elem.attribute("stroke-style") == "dash")
				toGen << "\tpen.setStyle(Qt::DashLine);\n";
			if (elem.attribute("stroke-style") == "dashdot")
				toGen << "\tpen.setStyle(Qt::DashDotLine);\n";
			if (elem.attribute("stroke-style") == "dashdotdot")
				toGen << "\tpen.setStyle(Qt::DashDotDotLine);\n";
			if (elem.attribute("stroke-style") == "none")
				toGen << "\tpen.setStyle(Qt::NoPen);\n";
		}

		if (elem.hasAttribute("fill-style"))
		{
			if (elem.attribute("fill-style")=="none")
				toGen << "\tbrush.setStyle(Qt::NoBrush);\n";
			else if(elem.attribute("fill-style")=="solid")
				toGen << "\tbrush.setStyle(Qt::SolidPattern);\n";
		}




		if (!elem.hasAttribute("stroke"))
			toGen << "\tpen.setColor(QColor(0,0,0));\n";
		if (!elem.hasAttribute("fill"))
			toGen << "\tbrush.setColor(QColor(255,255,255));\n";

		toGen << "\n\tpainter->setFont(font);\n";
		toGen << "\tpainter->setPen(pen);\n";
		toGen << "\tpainter->setBrush(brush);\n";

	}
}


void SdfRenderer::noScale()
{
	mNeedScale = false;
}


SdfIconEngineV2::SdfIconEngineV2(const QString &file)
{
	mRenderer.load(file);
	mRenderer.noScale();
}

void SdfIconEngineV2::paint(QPainter* /*painter*/, const QRect &/*rect*/, QIcon::Mode /*mode*/, QIcon::State /*state*/)
{
}
