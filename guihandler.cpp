#include "guihandler.h"

#include <QObject>
#include <QQuickItem>
#include <QDir>
#include <QStringList>
#include <QFileInfoList>
#include <QStringListModel>
#include <QQmlContext>
#include <QDebug>

GuiHandler::GuiHandler(QQuickView *view, QObject *parent) :
	view(view), QObject(parent)
{
	loaded = false, loading = false, sorting = false;
	preamp = 0;

	// path to the config folder
	configPath = "C:/Program Files/EqualizerAPO/config";
	excludeConfigs << "config" << "demo" << "example" << "multichannel"
				   << "Flat" << "User Defined";
	// filters = new FilterFile(this, &configPath);
}

/* PUBLIC */
// wrapper function
void GuiHandler::load()
{
	loadEQFiles();
}


/* SLOTS */
// toggles whether the equalizer is on or off
void GuiHandler::on_powerToggled(bool checked)
{

}

// updates the preamp value
void GuiHandler::on_preampChanged(double value)
{

}

// updates the settings when the preset is changed
void GuiHandler::on_comboChanged(const QString &text)
{
	comboBox->setProperty("currentIndex", configList.indexOf(text));
}

// resets everything back to the Flat preset
void GuiHandler::on_resetButtonClicked()
{
	on_comboChanged("Flat");
}

// saves the current settings as a new or existing preset
void GuiHandler::on_saveButtonClicked()
{

}

// deletes the current preset
void GuiHandler::on_deleteButtonClicked()
{

}

// updates the user-defined preset to reflect real-time changes to a band's volume
void GuiHandler::on_bandVolumeChanged(int index, double value)
{

}

// updates the user-defined preset to reflect real-time changes to a band's frequency and
// reorganizes it so that the frequencies are ascending and unused bands are to the right
void GuiHandler::on_bandFreqChanged(int index, double value)
{

}


/* PRIVATE */
// adds detected presets to the combo box
void GuiHandler::loadEQFiles()
{
	QDir configDir(configPath);
	QStringList nameFilter("*.txt");
	QFileInfoList configs(configDir.entryInfoList(nameFilter, QDir::Files, QDir::Name));
	configList << "Flat";

	for (int i = 0; i < configs.size(); i++){
		QString fileName = configs.at(i).baseName();
		fileName.replace(" ", "_");
		if (!excludeConfigs.contains(fileName, Qt::CaseInsensitive)){
			configList << fileName.replace("_", " ");
		}
	}

	qDebug() << configList;

	view->rootContext()->setContextProperty("comboBoxModel", configList);
	on_comboChanged("N-ergy");
}

// loads the settings for the active preset
void GuiHandler::loadSettings(bool setIndex)
{

}

// swaps the values of two filters
void GuiHandler::swapBand(int oldBandNum, int newBandNum)
{

}
