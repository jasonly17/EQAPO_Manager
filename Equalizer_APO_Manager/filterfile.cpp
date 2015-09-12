#include "filterfile.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

FilterFile::FilterFile(QWidget *parent, QString *path) :
	QWidget(parent)
{
	version = "1.0";
	configPath = path;
	initEQFile();
}

// public functions
// change whether equalization is on or off
bool FilterFile::toggleEQ(bool state)
{
	if (!state)
	{
		qDebug() << "off";
		QFile file(*configPath + "/config.txt");
		if (file.exists())
		{
			return file.rename(*configPath + "/config.bkp");
		}
	}
	else
	{
		qDebug() << "on";
		QFile file(*configPath + "/config.bkp");
		QFile oldFile(*configPath + "/config.txt");
		if (file.exists() && oldFile.exists())
		{
			oldFile.remove();
			if (file.copy(*configPath + "/config.txt"))
			{
				file.remove();
				return true;
			}
		}
	}
	return false;
}

// displays a dialog with information for the user
int FilterFile::filterDialog(QString type, QWidget *parent, QString msg, QString *objectName)
{
	if (type == "save")
	{
		int button = -1;
		QInputDialog *dialog = new QInputDialog(parent, Qt::Dialog);
		dialog->setWindowTitle(parentWidget()->windowTitle());
		dialog->setLabelText("Enter a name for your configuration: ");
		dialog->setInputMode(QInputDialog::TextInput);
		while (button == -1)
		{
			int ret = dialog->exec();
			switch (ret){
				case QMessageBox::Accepted:
					*objectName = dialog->textValue().remove(
								QRegExp(QString::fromUtf8("[`~!@#$%^&*()_+=|:;<>«»,.?/{}\'\"\\[\\]\\\\]")));
					qDebug() << *objectName;
					if (!objectName->isEmpty() && objectName->length() <= 20)
					{
						objectName->replace(" ", "_");
						button = 1;
					}
					else if (objectName->length() > 20)
					{
						QMessageBox::warning(qobject_cast<QWidget *>(dialog), parentWidget()->windowTitle(),
											 "The configuration name you entered is too long. Please enter a shorter name.");
					}
					else
					{
						QMessageBox::warning(qobject_cast<QWidget *>(dialog), parentWidget()->windowTitle(),
											 "You didn't enter a value configuration name. Please enter in a new name.");
					}
					break;
				case QMessageBox::Cancel:
					button = 0;
					break;
				default:
					button = 0;
					break;
			}
		}
		delete dialog;
		return button;
	}
	else if (type == "overwrite")
	{
		int button = -1;
		QMessageBox *dialog = new QMessageBox(parent);
		dialog->setText("There is already a configuration named \"" + msg + "\". Do you want to overwrite the existing configuration file?");
		dialog->addButton(QMessageBox::Discard);
		dialog->addButton(QMessageBox::Cancel);
		dialog->addButton(QMessageBox::Save);
		int ret = dialog->exec();
		switch (ret){
			case QMessageBox::Cancel:
				button = 0;
				break;
			case QMessageBox::Save:
				button = 1;
				break;
			case QMessageBox::Discard:
				button = 2;
				break;
			default:
				break;
		}
		delete dialog;
		return button;
	}
	else if (type == "delete")
	{
		int button = -1;
		QMessageBox *dialog = new QMessageBox(parent);
		dialog->setText("Are you sure you want to delete \"" + msg + "\"? This cannot be undone.");
		dialog->addButton(QMessageBox::Cancel);
		dialog->addButton(QMessageBox::Ok);
		int ret = dialog->exec();
		switch (ret){
			case QMessageBox::Cancel:
				button = 0;
				break;
			case QMessageBox::Ok:
				button = 1;
				break;
			default:
				break;
		}
		delete dialog;
		return button;
	}
	return -1;
}

// retrieve preamp value and active config from config.txt; called only at start
bool FilterFile::getEQ(int *preamp, QString *activeConfig, QStringList *excludeConfigs)
{
	QFile file(*configPath + "/config.txt");
	if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString contents = in.readLine();
			if (contents.left(8) == "Preamp: ")
			{
				QString value = contents.mid(8);
				*preamp = value.left(value.length() - 3).toInt();
			}
			else if (contents.left(9) == "Include: ")
			{
				QString value = contents.mid(9);
				QString configName = value.left(value.length() - 4);
				if (!excludeConfigs->contains(configName, Qt::CaseInsensitive))
				{
					*activeConfig = configName;
				}
				else
				{
					*activeConfig = "Flat";
				}
			}
		}
		file.close();
		return true;
	}
	return false;
}

// update the configuration file
bool FilterFile::changeEQ(int preamp, QString *activeConfig)
{
	QFile file(*configPath + "/config.txt");
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QString configName = *activeConfig;
		if (!configName.isEmpty())
		{
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

// save the current filter settings
int FilterFile::saveEQ(QString *lastConfig, QString *newConfig)
{
	QFile oldFile(*configPath + "/" + *lastConfig + ".txt");
	QFile newFile(*configPath + "/" + *newConfig + ".txt");
	if (oldFile.exists())
	{
		if (!newFile.exists())
		{
			return oldFile.rename(*configPath + "/" + *lastConfig + ".txt", *configPath + "/" + *newConfig + ".txt");
		}
		else
		{
			int ret = filterDialog("overwrite", parentWidget(), *newConfig);
			if (ret == 1)
			{
				newFile.remove();
				return oldFile.rename(*configPath + "/" + *lastConfig + ".txt", *configPath + "/" + *newConfig + ".txt");
			}
			return ret;
		}
	}
	return -1;
}

// remove the current filter settings
bool FilterFile::removeEQ(QString *configName)
{
	QFile file(*configPath + "/" + *configName + ".txt");
	if (file.exists())
	{
		int ret = filterDialog("delete", parentWidget(), *configName);
		qDebug() << ret;
		if (ret == 1)
		{
			return file.remove();
		}
	}
	return false;
}

// write the current filter settings to the config folder
void FilterFile::writeEQFile(QString *configName, QMap<int, QSpinBox *> *freqValues,
							 QMap<int, QDoubleSpinBox *> *levelValues)
{
	QFile file(*configPath + "/" + *configName + ".txt");
	qDebug() << file.fileName();
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		// write out basic info
		out << "Filter Settings File" << endl << endl;
		out << "Equalizer APO " << version << endl << endl;
		out << "Notes: " << *configName << endl << endl;
		out << "Equaliser: Generic" << endl;
		out << "No measurement" << endl;

		// write out filter settings
		for (int filterNum = 1; filterNum <= 20; filterNum++)
		{
			int freqValue = freqValues->value(filterNum)->value();
			double levelValue = levelValues->value(filterNum)->value();
			QString filter;
			if (freqValue == 0)
			{
				filter = QString("Filter %1: ON  %2").arg(filterNum, 2).arg("None", -8);
			}
			else
			{
				filter = QString("Filter %1: ON  %2 Fc%3 Hz  Gain%4 dB  Q%5").arg(filterNum, 2)
						.arg("PK", -8).arg(freqValue, 8).arg(levelValue, 6).arg("1.00", 6);
			}

			out << filter << endl;
		}
		file.close();
	}
}

// read values from the filter settings file
QMap<int, QList<double> > FilterFile::readEQFile(QString *configName)
{
	QMap<int, QList<double> > settings;
	QFile file(*configPath + "/" + *configName + ".txt");
	qDebug() << file.fileName();
	if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString contents = in.readLine();
			if (contents.left(7) == "Filter ")
			{
				int filterNum = 0;
				QRegExp rx("\\b(\\d+,?.?\\d*)\\b");
				QList<double> values;
				int pos = 0;
				while ((pos = rx.indexIn(contents, pos)) != -1)
				{
					QString match = rx.cap(1);
					match.replace(",", "");
					if (filterNum == 0)
					{
						filterNum = match.toInt();
					}
					else
					{
						values << match.toDouble();
					}
					pos += rx.matchedLength();
				}

				if (contents.mid(15, 4) == "None")
				{
					values << 0.0 << 0.0;
				}

				if (filterNum != 0)
				{
					settings.insert(filterNum, values);
				}
			}
		}
		file.close();
	}
	return settings;
}


// private functions
// create a default Flat config that does not equalize the sound
void FilterFile::initEQFile()
{
	QFile file(*configPath + "/Flat.txt");
	if (!file.exists())
	{
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream out(&file);
			// write out basic info
			out << "Filter Settings File" << endl << endl;
			out << "Equalizer APO " << version << endl << endl;
			out << "Notes: Flat" << endl << endl;
			out << "Equaliser: Generic" << endl;
			out << "No measurement" << endl;

			// write out filter settings
			for (int filterNum = 1; filterNum <= 20; filterNum++)
			{
				QString filter = QString("Filter %1: ON  %2").arg(filterNum, 2).arg("None", -8);
				out << filter << endl;
			}
			file.close();
		}
	}
}
