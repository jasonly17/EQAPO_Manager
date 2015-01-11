#include "guihandler.h"

#include <QObject>
#include <QQuickItem>
#include <QQmlEngine>
#include <QDir>
#include <QStringList>
#include <QFileInfoList>
#include <QStringListModel>
#include <QQmlContext>
#include <QDebug>

GuiHandler::GuiHandler(QQuickWindow *window, QObject *parent) :
	window(window), QObject(parent)
{
	loaded = false, loading = false, sorting = false;
	preamp = 0;

	// path to the config folder
	configPath = "C:/Program Files/EqualizerAPO/config";
	excludeConfigs << "config" << "demo" << "example" << "multichannel"
				   << "Flat" << "User_Defined";
	eqFuncs = new EqualizerFuncs(&configPath, window);
	connect(eqFuncs, &EqualizerFuncs::result,
			[this](QString text){
		if (text.isEmpty()){
			int newIndex = configList.indexOf(activeConfig) - 1;
			configList.removeAt(newIndex + 1);
			activeConfig = configList.at(newIndex);
			qmlEngine(this->window)->rootContext()->setContextProperty("comboBoxModel", configList);
			comboBox->setProperty("currentIndex", newIndex);
		}
		else {
			activeConfig = text;
			loadEQFiles();
			comboBox->setProperty("currentIndex", configList.indexOf(text));
		}
		qDebug() << configList;
	});
}

/* PUBLIC */
// wrapper function
void GuiHandler::load()
{
	loadEQFiles();
	if (eqFuncs->getConfig(&preamp, &activeConfig, &excludeConfigs)){
		if (activeConfig.isEmpty()){
			window->findChild<QQuickItem*>("powerButton")->setProperty("checked", false);
		}
		loadSettings(true);
	}
	loaded = true;
}


/* SLOTS */
// toggles whether the equalizer is on or off
void GuiHandler::powerToggled(bool checked)
{
	eqFuncs->toggleEQ(checked);
}

// updates the preamp value
void GuiHandler::preampChanged(double value)
{
	preamp = value;
	eqFuncs->changeConfig(preamp, activeConfig);
}

// updates the settings when the preset is changed
void GuiHandler::comboBoxChanged(const QString &text)
{
	if (loaded && !loading){
		loading = true;
		activeConfig = text;
		loadSettings();
		loading = false;
	}
}

// resets everything back to the Flat preset
void GuiHandler::resetButtonClicked()
{
	activeConfig = "Flat";
	comboBox->setProperty("currentIndex", 0);
}

// saves the current settings as a new or existing preset
void GuiHandler::saveButtonClicked()
{
	eqFuncs->displayDialog(EqualizerFuncs::SaveDialog, window, activeConfig);
}

// deletes the current preset
void GuiHandler::deleteButtonClicked()
{
	if (activeConfig != "Flat"){
		eqFuncs->displayDialog(EqualizerFuncs::DeleteDialog, window, activeConfig);
	}
}

// updates the user-defined preset to reflect real-time changes to a band
void GuiHandler::bandChanged()
{
	if (loaded && !loading){
		if (activeConfig != "User Defined"){
			if (configList.indexOf("User Defined") == -1){
				configList << "User Defined";
			}
			qDebug() << configList;
			QMap<int, QList<double> > bandValues = getBandValues();
			activeConfig = "User Defined";
			eqFuncs->writeEQFile("User_Defined", &bandValues);
			qmlEngine(window)->rootContext()->setContextProperty("comboBoxModel", configList);
			comboBox->setProperty("currentIndex", configList.count() - 1);
		}
		else {
			eqFuncs->writeEQFile("User_Defined", &getBandValues());
		}
	}
}


/* PRIVATE */
// adds detected presets to the combo box
void GuiHandler::loadEQFiles()
{
	loading = true;
	QDir configDir(configPath);
	QStringList nameFilter("*.txt");
	QFileInfoList configs(configDir.entryInfoList(nameFilter, QDir::Files, QDir::Name));
	configList.clear();
	configList << "Flat";

	bool userConfig = false;
	for (int i = 0; i < configs.size(); i++){
		QString fileName = configs.at(i).baseName().replace(" ", "_");
		if (!excludeConfigs.contains(fileName, Qt::CaseInsensitive)){
			configList << fileName.replace("_", " ");
		}
		else if (fileName == "User_Defined"){
			userConfig = true;
		}
	}
	if (userConfig){
		configList << "User Defined";
	}

	qDebug() << configList;
	qmlEngine(window)->rootContext()->setContextProperty("comboBoxModel", configList);
	loading = false;
}

// loads the settings for the active preset
void GuiHandler::loadSettings(bool setIndex)
{
	if (eqFuncs->changeConfig(preamp, activeConfig)){
		QQuickItem *preampSlider = window->findChild<QQuickItem*>("preampSlider");
		preampSlider->setProperty("value", qMax(-30.0, qMin(0.0, preamp)));
		if (setIndex){
			int index = configList.indexOf(activeConfig);
			if (index != -1){
				comboBox->setProperty("currentIndex", index);
			}
		}

		QString configName = activeConfig;
		QMap<int, QList<double> > settings = eqFuncs->readEQFile(configName.replace(" ", "_"));
		QMapIterator<int, QList<double> > itr(settings);
		while (itr.hasNext()){
			itr.next();
			QQuickItem* item;
			QMetaObject::invokeMethod(bands, "itemAt",
									  Q_RETURN_ARG(QQuickItem*, item), Q_ARG(int, itr.key()));

			if (itr.value().at(0) == 50000.0){
				item->findChild<QQuickItem*>("freqValue")->setProperty("text", "0");
				item->findChild<QQuickItem*>("volumeSlider")->setProperty("value", 0);
			}
			else {
				item->findChild<QQuickItem*>("freqValue")->setProperty("text", itr.value().at(0));
				item->findChild<QQuickItem*>("volumeSlider")->setProperty("value", itr.value().at(1));
			}
		}
		qDebug() << "Done Loading";
	}
}

// returns the frequency and volume of all the bands
QMap<int, QList<double> > GuiHandler::getBandValues()
{
	QMap<int, QList<double> > settings;
	for (int bandNum = 0; bandNum < bands->property("model").toInt(); bandNum++){
		QList<double> values;

		QQuickItem* item;
		QMetaObject::invokeMethod(bands, "itemAt",
								  Q_RETURN_ARG(QQuickItem*, item), Q_ARG(int, bandNum));

		values.append(item->findChild<QQuickItem*>("freqValue")->property("text").toDouble());
		values.append(item->findChild<QQuickItem*>("volumeSlider")->property("value").toDouble());

		if (values.at(0) == 0.0){
			values.replace(0, 50000.0);
		}
		settings.insert(bandNum, values);
	}
	return settings;
}
