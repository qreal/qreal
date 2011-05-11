#include "diffViewer.h"

using namespace qReal::gui;

DiffViewer::DiffViewer(QWidget *parent) : QDialog(parent)
{
	setWindowModality(Qt::WindowModal);
	setWindowState(Qt::WindowMaximized);
	setWindowTitle("Please wait... Finding difference.");

	mOkButton = new QPushButton(tr("OK"), this);
	connect(mOkButton, SIGNAL(clicked()), this, SLOT(accept()));
}
