#pragma once

#include <QTableWidget>
#include "../../qrrepo/repoApi.h"
class ReferenceTypeWindow : public QTableWidget
{
	Q_OBJECT

public:
	ReferenceTypeWindow(QWidget *parent, qrRepo::RepoApi mApi);
};
