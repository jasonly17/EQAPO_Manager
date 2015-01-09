#include "windowfunc.h"

WindowFunc::WindowFunc(QCursor* cursor, QQuickView *view, QObject *parent) :
	view(view), cursor(cursor), QObject(parent)
{
}

/* PUBLIC */
// sets up the window for moving
void WindowFunc::beginMoveWindow(int x, int y, int width, int height)
{
	lastGeometry = QRect(view->framePosition(), QSize(width, height));
	lastPos = QPoint(x, y);
	dragDist = QPoint(0, 0);
}

// moves the window based on the current cursor position
void WindowFunc::moveWindow(int x, int y)
{
	QPoint relPoint(x - lastPos.x(), y - lastPos.y());
	QPoint oldPoint = view->framePosition();
	view->setFramePosition(QPoint(oldPoint.x() + relPoint.x(), oldPoint.y() + relPoint.y()));

}

// minimizes the window to the taskbar
void WindowFunc::minimizeWindow()
{
	view->setWindowState(Qt::WindowMinimized);
}

// closes the window
void WindowFunc::closeWindow()
{
	view->close();
}
