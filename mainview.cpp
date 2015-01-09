#include "mainview.h"

#include <QDir>
#include <QDebug>

//#include "filterfile.h"

MainView::MainView(QWindow *parent) :
	QQuickView(parent)
{
}


/* SLOTS */
// recieves messages sent from other instances of the program
void MainView::on_receiveMsg(QString msg)
{
	qDebug() << "receiving";
	if (msg == "FOCUS"){
		this->show();
	}
}
