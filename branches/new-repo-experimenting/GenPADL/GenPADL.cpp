#include "GenPADL.h"

#include <QtGui>

using namespace GenPADL;

void PADLGenerator::Generate() const
{
  QMessageBox MessageBox(QMessageBox::Information
    , "Something works..."
    , "Dummy generator called successfully."
    , QMessageBox::Ok
  );
  MessageBox.exec();
} 

