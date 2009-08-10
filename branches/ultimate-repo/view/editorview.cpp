/** @file editorview.cpp
 * 	@brief Класс, реализующий представление в схеме Model/View 
 * */
#include <QtGui>
#include "editorview.h"

EditorView::EditorView(QWidget *parent) : QGraphicsView(parent)
{
	setRenderHint(QPainter::Antialiasing, true);
}

EditorView::~EditorView()
{
}

void EditorView::zoomIn()
{
	scale(1.5,1.5);
}

void EditorView::zoomOut()
{
	scale(0.666,0.666);
}
