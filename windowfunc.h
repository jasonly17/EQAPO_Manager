#ifndef WINDOWFUNC_H
#define WINDOWFUNC_H

#include <QObject>
#include <QQuickView>
#include <QCursor>

class WindowFunc : public QObject
{
	Q_OBJECT
public:
	explicit WindowFunc(QCursor *cursor, QQuickView *view, QObject *parent = 0);
	Q_INVOKABLE void beginMoveWindow(int x, int y, int width, int height);
	Q_INVOKABLE void moveWindow(int x, int y);
	Q_INVOKABLE void minimizeWindow();
	Q_INVOKABLE void closeWindow();

private:
	QQuickView *view;
	QCursor *cursor;
	QPoint lastPos, dragDist;
	QRect lastGeometry;
};

#endif // WINDOWFUNC_H
