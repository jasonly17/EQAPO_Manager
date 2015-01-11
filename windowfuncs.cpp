#include "windowfuncs.h"

WindowFuncs::WindowFuncs(QCursor* cursor, QQuickWindow *window, QObject *parent) :
	window(window), cursor(cursor), QObject(parent)
{
}

/* PUBLIC */
// sets up the window for moving
void WindowFuncs::beginMoveWindow(int x, int y, int width, int height)
{
	lastGeometry = QRect(window->framePosition(), QSize(width, height));
	lastPos = QPoint(x, y);
	dragDist = QPoint(0, 0);
}

// moves the window based on the current cursor position
void WindowFuncs::moveWindow(int x, int y)
{
	QPoint relPoint(x - lastPos.x(), y - lastPos.y());
	QPoint oldPoint = window->framePosition();
	window->setFramePosition(QPoint(oldPoint.x() + relPoint.x(), oldPoint.y() + relPoint.y()));

}

// minimizes the window to the taskbar
void WindowFuncs::minimizeWindow()
{
	window->setWindowState(Qt::WindowMinimized);
}

// closes the window
void WindowFuncs::closeWindow()
{
	window->close();
}
