#include "equalizerfuncs.h"

#include <QFile>
#include <QQuickItem>
#include <QList>
#include <QDebug>

EqualizerFuncs::EqualizerFuncs(QString *configPath, QObject *parent) :
	configPath(*configPath), QObject(parent)
{
	version = "1.0";
	invalidChars = QRegExp(QString::fromUtf8("[`~!@#$%^&*()_+=|:;<>«»,.?/{}\'\"\\[\\]\\\\]"));
	initEQFile();
}

/* PUBLIC */
// controls the toggling of the equalizer, saving the previous state
void EqualizerFuncs::toggleEQ(bool state)
{
	if (!state){
		qDebug() << "off";
		QFile file(configPath + "/config.txt");
		if (file.exists()){
			file.rename(configPath + "/config.bkp");
			QFile newFile(configPath + "/config.txt");
			if (newFile.open(QIODevice::WriteOnly | QIODevice::Text)){
				QTextStream out(&newFile);
				out << "Preamp: 0 dB" << endl;
				out << "Include: " << endl;
				file.close();
			}
		}
	}
	else {
		qDebug() << "on";
		QFile file(configPath + "/config.bkp");
		QFile oldFile(configPath + "/config.txt");
		if (file.exists()){
			if (oldFile.exists()){
				oldFile.remove();
			}
			if (file.copy(configPath + "/config.txt")){
				file.remove();
			}
		}
	}
}

// displays a dialog with information for the user
void EqualizerFuncs::displayDialog(int type, QQuickWindow *window,
								   QString configName, QString lastConfig)
{
	QQmlEngine *engine = qmlEngine(window);

	QUrl path;
	if (type == EqualizerFuncs::SaveDialog){
		path.setUrl("qrc:/SaveDialog.qml");
	}
	else if (type == EqualizerFuncs::OverwriteDialog){
		path.setUrl("qrc:/OverwriteDialog.qml");
	}
	else {
		path.setUrl("qrc:/DeleteDialog.qml");
	}

	QQmlComponent component(engine, path);
	if (component.status() != component.Ready){
		if (component.status() == component.Error){
			qDebug() << component.errorString();
		}
	}
	dialog = qobject_cast<QQuickWindow*>(component.create());
	dialog->setTitle("Equalizer APO Manager");
	dialog->setFlags(Qt::Window | Qt::FramelessWindowHint);
	dialog->setIcon(QIcon(":/Resources/Icon.png"));
	dialog->setModality(Qt::ApplicationModal);

	dialog->setProperty("baseColor", window->property("baseColor"));
	dialog->setProperty("fontColor", window->property("fontColor"));

	connect(dialog, SIGNAL(cancelButtonClicked(int)),
			this, SLOT(cancelButtonClicked(int)));

	if (type == EqualizerFuncs::SaveDialog){
		dialog->setProperty("lastConfig", configName);

		connect(dialog, SIGNAL(okButtonClicked(int, QString, QString)),
				this, SLOT(okButtonClicked(int, QString, QString)));
	}
	else if (type == EqualizerFuncs::OverwriteDialog){
		dialog->setProperty("configName", configName);

		connect(dialog, SIGNAL(yesButtonClicked(int, QString, QString)),
				this, SLOT(okButtonClicked(int, QString, QString)));
		connect(dialog, SIGNAL(discardButtonClicked(int)),
				this, SLOT(discardButtonClicked(int)));
	}
	else {
		dialog->setProperty("configName", configName);

		connect(dialog, SIGNAL(yesButtonClicked(int, QString)),
				this, SLOT(okButtonClicked(int, QString)));
	}
	dialog->show();
}

// gets the saved preamp value and active config from config.txt
bool EqualizerFuncs::getConfig(double *preamp, QString *activeConfig, QStringList *excludeConfigs)
{
	QFile file(configPath + "/config.txt");
	if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream in(&file);
		while (!in.atEnd()){
			QString contents = in.readLine();
			if (contents.left(8) == "Preamp: "){
				QString text = contents.mid(8);
				*preamp = text.left(text.length() - 3).toInt();
			}
			else if (contents.left(9) == "Include: "){
				QString text = contents.mid(9);
				QString configName = text.left(text.length() - 4);
				if (!excludeConfigs->contains(configName, Qt::CaseInsensitive)
						|| configName == "User_Defined"){
					*activeConfig = configName.replace("_", " ");
				}
				else {
					*activeConfig = "Flat";
				}
			}
		}
		file.close();
		return true;
	}
	return false;
}

// updates the configuration file
bool EqualizerFuncs::changeConfig(double preamp, QString activeConfig)
{
	QFile file(configPath + "/config.txt");
	if (file.exists() && file.open(QIODevice::WriteOnly | QIODevice::Text)){
		QString configName = activeConfig;
		configName.replace(" ", "_");
		if (!configName.isEmpty()){
			configName.append(".txt");
		}
		QTextStream out(&file);
		out << "Preamp: " << preamp << " dB" << endl;
		out << "Include: " << configName << endl;
		file.close();
		return true;
	}
	return false;
}

// reads the settings from the specified preset file
QMap<int, QList<double> > EqualizerFuncs::readEQFile(QString configName)
{
	QMap<int, QList<double> > settings;
	QFile file(configPath + "/" + configName + ".txt");
	if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream in(&file);
		while (!in.atEnd()){
			QString contents = in.readLine();
			if (contents.left(7) == "Filter "){
				int band = -1;
				QList<double> values;

				if (contents.mid(15, 4) == "None"){
					values.append(50000.0);
					settings.insert(settings.count(), values);
				}
				else {
					QRegExp rx("\\s(\\-?\\d+,?.?\\d*)\\b");
					int pos = 0;
					while ((pos = rx.indexIn(contents, pos)) != -1){
						QString match = rx.cap(1).replace(",", " ");
						if (band == -1){
							band = match.toInt() - 1;
						}
						else if (values.count() == 0){
							values.append(match.toDouble());
						}
						else if(values.count() == 1){
							values.append(match.toDouble());
						}
						else {
							settings.insert(band, values);
							break;
						}
						pos += rx.matchedLength();
					}
				}
			}
		}
		file.close();
	}
	return settings;
}

// writes the current preset settings to the config folder
void EqualizerFuncs::writeEQFile(QString configName, QMap<int, QList<double> > *settings)
{
	QFile file(configPath + "/" + configName + ".txt");
	if (file.exists()){
		file.remove();
	}
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
		QTextStream out(&file);

		out << "Filter Settings File" << endl << endl;
		out << "Equalizer APO " << version << endl << endl;
		out << "Notes: " << configName << endl << endl;
		out << "Equaliser: Generic" << endl;
		out << "No measurement" << endl;

		QMapIterator<int, QList<double> > itr(*settings);
		while (itr.hasNext()){
			itr.next();
			QString band;
			if (itr.value().at(0) == 50000.0){
				band = QString("Filter %1: ON  %2").arg(itr.key() + 1, 2).arg("None", -8);
			}
			else {
				band = QString("Filter %1: ON  %2 Fc%3 Hz  Gain%4 dB  Q%5")
						.arg(itr.key() + 1, 2).arg("PK", -8).arg(itr.value().at(0), 8)
						.arg(itr.value().at(1), 6).arg("1.00", 6);
			}
			out << band << endl;
		}
		file.close();
	}
}


/* PUBLIC SLOTS */
// handles when the ok button on a dialog is clicked
void EqualizerFuncs::okButtonClicked(int type, QString textInput, QString lastConfig)
{
	if (type == EqualizerFuncs::MessageDialog){
		if (subDialog->property("message").toString().size() == 34){
			dialog->deleteLater();
		}
		subDialog->deleteLater();
	}
	else if (type == EqualizerFuncs::SaveDialog){
		QString message = "";
		if (textInput.isEmpty()){
			message = QStringLiteral("You did not enter a preset name. ")
					+ QStringLiteral("Please enter a valid preset name.");
		}
		else if (textInput == lastConfig){
			message = QStringLiteral("The preset has already been saved!");
		}
		else {
			message = QStringLiteral("The preset name you entered in invalid. ")
					+ QStringLiteral("Please enter a valid preset name.");
		}

		if (textInput.isEmpty() || textInput == lastConfig || textInput.contains(invalidChars)){
			QQmlEngine *engine = qmlEngine(dialog);
			QQmlComponent component(engine, QUrl("qrc:/MessageDialog.qml"));
			if (component.status() != component.Ready){
				if (component.status() == component.Error){
					qDebug() << component.errorString();
				}
			}
			subDialog = qobject_cast<QQuickWindow*>(component.create());
			subDialog->setTitle("Equalizer APO Manager");
			subDialog->setFlags(Qt::Window | Qt::FramelessWindowHint);
			subDialog->setIcon(QIcon(":/Resources/Icon.png"));
			subDialog->setModality(Qt::ApplicationModal);

			subDialog->setProperty("baseColor", dialog->property("baseColor"));
			subDialog->setProperty("fontColor", dialog->property("fontColor"));
			subDialog->setProperty("message", message);

			connect(subDialog, SIGNAL(okButtonClicked(int)),
					this, SLOT(okButtonClicked(int)));

			subDialog->show();
		}
		else {
			dialog->deleteLater();
			QFile oldFile(configPath + "/" + lastConfig.replace(" ", "_") + ".txt");
			QFile newFile(configPath + "/" + textInput.replace(" ", "_") + ".txt");
			qDebug() << lastConfig << textInput;
			if (oldFile.exists()){
				if (!newFile.exists()){
					qDebug() << "save";
					oldFile.rename(configPath + "/" + lastConfig + ".txt",
								   configPath + "/" + textInput + ".txt");
				}
				else {
					qDebug() << "overwrite";
					displayDialog(EqualizerFuncs::OverwriteDialog, dialog, textInput, lastConfig);
				}
			}
			emit result(textInput.replace("_", " "));
		}
	}
	else if (type == EqualizerFuncs::OverwriteDialog){
		dialog->deleteLater();
		QFile oldFile(configPath + "/" + lastConfig + ".txt");
		QFile newFile(configPath + "/" + textInput + ".txt");
		newFile.remove();
		oldFile.rename(configPath + "/" + lastConfig + ".txt", configPath + "/" + textInput + ".txt");
		emit result(textInput);
	}
	else {
		dialog->deleteLater();
		QFile file(configPath + "/" + textInput.replace(" ", "_") + ".txt");
		if (file.exists()){
			file.remove();
			emit result("");
		}
	}
}

// handles when the discard button on a dialog is clicked
void EqualizerFuncs::discardButtonClicked()
{
	subDialog->deleteLater();
}

// handles when the cancel button on a dialog is clicked
void EqualizerFuncs::cancelButtonClicked(int type)
{
	if (type == EqualizerFuncs::OverwriteDialog){
		subDialog->deleteLater();
	}
	dialog->deleteLater();
}


/* PRIVATE */
// creates a default config that does not equalize the sound
void EqualizerFuncs::initEQFile()
{
	QFile file(configPath + "/Flat.txt");
	if (!file.exists() && file.open(QIODevice::WriteOnly | QIODevice::Text)){
		QTextStream out(&file);

		out << "Filter Settings File" << endl << endl;
		out << "Equalizer APO " << version << endl << endl;
		out << "Notes: Flat" << endl << endl;
		out << "Equaliser: Generic" << endl;
		out << "No measurement" << endl;

		for (int bandNum = 1; bandNum <= 20; bandNum++){
			QString band = QString("Filter %1: ON  %2").arg(bandNum, 2).arg("None", -8);
			out << band << endl;
		}
		file.close();
	}
}
