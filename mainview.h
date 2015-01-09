#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QQuickView>

//#include "filterfile.h"

class MainView : public QQuickView
{
	Q_OBJECT
public:
	explicit MainView(QWindow *parent = 0);

public slots:
	void on_receiveMsg(QString msg);

private:

};

#endif // MAINVIEW_H
