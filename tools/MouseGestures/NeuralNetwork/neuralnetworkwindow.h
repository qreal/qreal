#pragma once
#include <QMainWindow>
#include "neuralnetwork.h"
#include "classifier.h"
#include "paintmanager.h"

namespace Ui {
	class NeuralNetworkWindow;
}

class NeuralNetworkWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit NeuralNetworkWindow(QWidget *parent = 0);
	~NeuralNetworkWindow();

private:
	Ui::NeuralNetworkWindow *ui;
        NeuralNetwork mNeuralNetwork;
        PaintManager * mPaintManager;
        QMap<QString, QString> mGestures;

private slots:
	void learn();
	void test();
	void setRad();
	void clear();
        void paint();
};

