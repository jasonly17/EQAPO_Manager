#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filterfile.h"
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QSlider>
/*
#include <QSpinBox>
#include <QMenu>
*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_receiveMsg(QString msg);
	void on_levelValueChanged(QSlider *levelSlider, double value);
	void on_levelSliderChanged(QDoubleSpinBox *levelValue, int value);
	void on_freqValueChanged(int filterNum, int value);
	void on_equalizerCombo_currentIndexChanged(const QString &arg1);
	void on_preampValue_valueChanged(int arg1);
	void on_powerButton_toggled(bool checked);
	void on_saveButton_clicked();
	void on_deleteButton_clicked();

private:
	bool _isLoaded = false;
	bool _loadingEQ = false;
	bool _sorting = false;

	Ui::MainWindow *ui;
	QSystemTrayIcon *trayIcon;
	QIcon *icon;
	QMenu *trayMenu;
	QMenu *configTrayMenu;
	QList<QMetaObject::Connection> connections;
	QList<QMetaObject::Connection> configTrayConnections;
	QString configPath;
	QStringList excludeConfigs;
	FilterFile *filters;
	int preamp = 0;
	QString activeConfig;
	QMap<int, QSpinBox *> freqValues;
	QMap<int, QDoubleSpinBox *> levelValues;
	QMap<int, QSlider *> levelSliders;

	void showWindow(bool state);
	void addFiltersToTray();
	void linkSettings();
	void loadEQFiles();
	void loadSettings(bool setIndex = false);
	void swapFilter(int oldFilterNum, int newFilterNum);
};

#endif // MAINWINDOW_H
