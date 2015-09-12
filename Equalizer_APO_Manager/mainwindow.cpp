#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filterfile.h"
#include <QMenu>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{	
	icon = new QIcon(":/Icon.png");
	qApp->setWindowIcon(*icon);
	ui->setupUi(this);
	ui->main->parentWidget()->setStyleSheet("QMainWindow {background: 'white';}");
	qApp->setQuitOnLastWindowClosed(false);
	configPath = "C:/Program Files/EqualizerAPO/config"; // path to the config folder
	excludeConfigs << "config" << "demo" << "example" << "multichannel" << "Flat" << "User Defined"; // ignore configs that are not supported or manually added
	linkSettings(); // collect objects from gui for easy access
	filters = new FilterFile(this, &configPath); // set up a FilterFile to handle file io operations
	loadEQFiles(); // load configs in config folder

	QMetaObject::Connection connection;
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(*icon);
	connection = connect(trayIcon, static_cast<void (QSystemTrayIcon::*)(QSystemTrayIcon::ActivationReason)>(&QSystemTrayIcon::activated),
						 [this](QSystemTrayIcon::ActivationReason reason){if (reason == QSystemTrayIcon::Trigger){showWindow(!isVisible());}});
	connections.append(connection);

	trayMenu = new QMenu(this);
	QAction *openTray = trayMenu->addAction("Open");
	configTrayMenu = trayMenu->addMenu("Configuration");
	addFiltersToTray();
	QAction *closeTray = trayMenu->addAction("Exit");

	connection = connect(openTray, &QAction::triggered, this, &MainWindow::showNormal);
	connections.append(connection);
	connection = connect(closeTray, &QAction::triggered, qApp, &QApplication::quit);
	connections.append(connection);

	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();

	connection = connect(ui->resetButton, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
						 [this](){on_equalizerCombo_currentIndexChanged("Flat"); ui->equalizerCombo->setCurrentIndex(0);});

	// link sliders and spinbox together
	for (int filterNum = 1; filterNum <= freqValues.size(); filterNum++)
	{
		QSpinBox *freqValue = freqValues.value(filterNum);
		QDoubleSpinBox *levelValue = levelValues.value(filterNum);
		QSlider *levelSlider = levelSliders.value(filterNum);
		connection = connect(levelValue, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
							 [this, levelSlider](double value){on_levelValueChanged(levelSlider, value);});
		connections.append(connection);

		connection = connect(levelSlider, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),
							 [this, levelValue](int value){on_levelSliderChanged(levelValue, value);});
		connections.append(connection);

		connection = connect(freqValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
							 [this, filterNum](int value){on_freqValueChanged(filterNum, value);});
		connections.append(connection);
	}

	// load current config
	if(filters->getEQ(&preamp, &activeConfig, &excludeConfigs))
	{
		if (activeConfig.isEmpty())
		{ui->powerButton->setChecked(false);}

		loadSettings(true);
	}

	_isLoaded = true; // prevents slots from accepting signals until settings loaded
	qDebug() << "Loaded";
}

MainWindow::~MainWindow()
{
	for (int i = 0; i < connections.size(); i++)
	{
		disconnect(connections[i]);
	}
	delete filters;
	delete trayIcon;
	delete trayMenu;
	delete icon;
	delete ui;
}


// private slots
// handle msgs from other instances
void MainWindow::on_receiveMsg(QString msg)
{
	qDebug() << "receiving";
	if (msg == "FOCUS")
	{
		MainWindow::activateWindow();
		MainWindow::showNormal();
	}
}

// links spinbox value to slider value
void MainWindow::on_levelValueChanged(QSlider *levelSlider, double value)
{
	if (_isLoaded && !_loadingEQ && activeConfig != "User Defined")
	{
		int index = ui->equalizerCombo->findText("User Defined");
		if (index == -1)
		{
			ui->equalizerCombo->addItem("User Defined");
			activeConfig = "User_Defined";
			filters->writeEQFile(&activeConfig, &freqValues, &levelValues);
			addFiltersToTray();
		}
		ui->equalizerCombo->setCurrentIndex(ui->equalizerCombo->count() - 1);
	}
	int num = value * 10;
	if (num != levelSlider->value())
	{
		levelSlider->setValue(num);
	}
	if (_isLoaded && !_loadingEQ)
	{
		filters->writeEQFile(&activeConfig, &freqValues, &levelValues);
	}
}

// links slider value to spinbox value
void MainWindow::on_levelSliderChanged(QDoubleSpinBox *levelValue, int value)
{
	double num = (double) value / 10;
	if (num != levelValue->value())
	{
		levelValue->setValue(num);
	}
}

// sorts frequency values from smallest to largest, keeping unused filters to the right
void MainWindow::on_freqValueChanged(int filterNum, int value)
{
	if (_isLoaded && !_sorting && !_loadingEQ)
	{
		if (activeConfig != "User Defined")
		{
			int index = ui->equalizerCombo->findText("User Defined");
			if (index == -1)
			{
				ui->equalizerCombo->addItem("User Defined");
				activeConfig = "User_Defined";
				filters->writeEQFile(&activeConfig, &freqValues, &levelValues);
				addFiltersToTray();
			}
			ui->equalizerCombo->setCurrentIndex(ui->equalizerCombo->count() - 1);
		}
		qDebug() << "Called";
		_sorting = true;
		while (filterNum >= 1 && filterNum <= 20)
		{
			qDebug() << "Looping" << filterNum;
			QSpinBox *prevFilter = 0;
			QSpinBox *nextFilter = 0;
			if (filterNum - 1 >= 1)
			{
				prevFilter = freqValues.value(filterNum - 1);
			}
			if (filterNum + 1 <= 20)
			{
				nextFilter = freqValues.value(filterNum + 1);
			}

			if (prevFilter != 0 && prevFilter->value() == 0)
			{
				swapFilter(filterNum, filterNum - 1);
				filterNum--;
			}
			else if (nextFilter != 0 && value == 0)
			{
				if (nextFilter->value() != 0)
				{
					levelValues.value(filterNum)->setValue(0);
					swapFilter(filterNum, filterNum + 1);
					filterNum++;
				}
				else
				{break;}
			}
			else if (prevFilter != 0 && value < prevFilter->value())
			{
				swapFilter(filterNum, filterNum - 1);
				filterNum--;
			}
			else if (nextFilter != 0 && nextFilter->value() != 0 && value > nextFilter->value())
			{
				swapFilter(filterNum, filterNum + 1);
			}
			else if ((prevFilter != 0 && value == prevFilter->value()) || (nextFilter != 0 && value == nextFilter->value()))
			{
				freqValues.value(filterNum)->setValue(0);
				levelValues.value(filterNum)->setValue(0);
				value = 0;
			}
			else
			{break;}
		}
		filters->writeEQFile(&activeConfig, &freqValues, &levelValues);
		_sorting = false;
	}
}

// change the filter file when the equalizer combo box is changed
void MainWindow::on_equalizerCombo_currentIndexChanged(const QString &value)
{
	if (_isLoaded && activeConfig != value)
	{
		_loadingEQ = true;
		activeConfig = value;
		activeConfig.replace(" ", "_");
		loadSettings();
		_loadingEQ = false;
	}
}

// change the preamp and update configuration file
void MainWindow::on_preampValue_valueChanged(int value)
{
	preamp = value;
	filters->changeEQ(preamp, &activeConfig);
}

// toggle if equalization is on or off
void MainWindow::on_powerButton_toggled(bool checked)
{
	qDebug() << "Toggle" << checked;
	qDebug() << filters->toggleEQ(checked);
	if (!checked)
	{
		QString noConfig = "";
		filters->changeEQ(0, &noConfig);}
}

// save current configuration under a new name
void MainWindow::on_saveButton_clicked()
{
	bool loop = true;
	QString lastConfig = activeConfig;
	while (loop)
	{
		if (filters->filterDialog("save", this, "", &activeConfig) == 1)
		{
			int ret = filters->saveEQ(&lastConfig, &activeConfig);
			qDebug() << ret;
			switch (ret){
				case 0: // Cancel
					break;
				case 1: // Save
				{
					QString configName = activeConfig;
					int index = ui->equalizerCombo->findText(configName.replace("_", " "));
					if (index != -1)
					{
						ui->equalizerCombo->setCurrentIndex(index);
						ui->equalizerCombo->removeItem(ui->equalizerCombo->findText(lastConfig.replace("_", " ")));
						addFiltersToTray();
					}
					else
					{
						ui->equalizerCombo->removeItem(ui->equalizerCombo->currentIndex());
						ui->equalizerCombo->addItem(configName);
						ui->equalizerCombo->setCurrentIndex(ui->equalizerCombo->findText(configName));
						addFiltersToTray();
					}
					filters->changeEQ(preamp, &activeConfig);
				}
					loop = false;
					break;
				case 2: // Discard
					activeConfig = lastConfig;
					loop = false;
					break;
				default:
					filters->filterDialog("error", this, "The configuration could not be saved.");
					activeConfig = lastConfig;
					loop = false;
					break;
			}
		}
		else
		{
			break;
		}
	}
}

// delete current configuration
void MainWindow::on_deleteButton_clicked()
{
	if (activeConfig != "Flat")
	{
		bool test = filters->removeEQ(&activeConfig);
		qDebug() << test;
		if(test)
		{
			ui->equalizerCombo->removeItem(ui->equalizerCombo->currentIndex());
			addFiltersToTray();
		}
	}
}


// private functions
// toggle the visiblity of the application
void MainWindow::showWindow(bool state)
{
	if (state)
	{
		MainWindow::showNormal();
	}
	else
	{
		MainWindow::hide();
	}
}

// adds existing filters to the tray configuration menu
void MainWindow::addFiltersToTray()
{
	for (int i = 0; i < configTrayConnections.size(); i++)
	{
		disconnect(configTrayConnections[i]);
	}
	configTrayMenu->clear();

	QMetaObject::Connection connection;
	for (int config = 0; config < ui->equalizerCombo->count(); config++)
	{
		QString configName = ui->equalizerCombo->itemText(config);
		QAction * configTray = configTrayMenu->addAction(configName);
		connection = connect(configTray, static_cast<void (QAction::*)(bool)>(&QAction::triggered),
							 [this, config](){ui->equalizerCombo->setCurrentIndex(config);});
		configTrayConnections.append(connection);
	}
}

// cache important gui elements for easy access
void MainWindow::linkSettings()
{
	for (int filterNum = 0; filterNum < ui->bandSliders->count(); filterNum++)
	{
		QLayout *group = ui->bandSliders->itemAt(filterNum)->layout();
		if (group)
		{
			for (int i = 0; i < group->count(); i++)
			{
				QWidget *item = group->itemAt(i)->widget();
				if (item)
				{
					if (item->objectName() == "freqValue_" + QString::number(filterNum + 1))
					{
						freqValues.insert(filterNum + 1, qobject_cast<QSpinBox *>(item));
					}
					else if (item->objectName() == "levelValue_" + QString::number(filterNum + 1))
					{
						levelValues.insert(filterNum + 1, qobject_cast<QDoubleSpinBox *>(item));
					}
					else if (item->objectName() == "levelSlider_" + QString::number(filterNum + 1))
					{
						levelSliders.insert(filterNum + 1, qobject_cast<QSlider *>(item));
					}
				}
			}
		}
	}
	qDebug() << "Linking done.";
}

// add found configs into the combobox
void MainWindow::loadEQFiles()
{
	QDir configDir(configPath);
	QStringList nameFilter("*.txt");
	QFileInfoList configs(configDir.entryInfoList(nameFilter, QDir::Files, QDir::Name));
	ui->equalizerCombo->addItem("Flat");

	for (int i = 0; i < configs.size(); i++)
	{
		QString fileName = configs.at(i).baseName();
		fileName.replace(" ", "_");
		if (!excludeConfigs.contains(fileName, Qt::CaseInsensitive))
		{
			ui->equalizerCombo->addItem(fileName.replace("_", " "));
		}
	}
}

// load values from current config
void MainWindow::loadSettings(bool setIndex)
{
	if (filters->changeEQ(preamp, &activeConfig))
	{
		ui->preampValue->setValue(preamp);
		if (setIndex)
		{
			QString configName = activeConfig;
			int index = ui->equalizerCombo->findText(configName.replace("_", " "));
			if (index != -1)
			{
				ui->equalizerCombo->setCurrentIndex(index);
			}
		}

		QMap<int, QList<double> > settings = filters->readEQFile(&activeConfig);
		for (int filterNum = 1; filterNum <= settings.count(); filterNum++)
		{
			QList<double> setting = settings.value(filterNum);
			freqValues.value(filterNum)->setValue(setting.at(0));
			levelValues.value(filterNum)->setValue(setting.at(1));
		}
		qDebug() << "Done Loading";
	}
}

// swap the values of two filters
void MainWindow::swapFilter(int oldFilterNum, int newFilterNum)
{
	double level = levelValues.value(newFilterNum)->value();
	int freq = freqValues.value(newFilterNum)->value();

	levelValues.value(newFilterNum)->setValue(levelValues.value(oldFilterNum)->value());
	freqValues.value(newFilterNum)->setValue(freqValues.value(oldFilterNum)->value());
	levelValues.value(oldFilterNum)->setValue(level);
	freqValues.value(oldFilterNum)->setValue(freq);
}
