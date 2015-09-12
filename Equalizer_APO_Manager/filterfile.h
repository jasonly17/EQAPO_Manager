#ifndef FILTERFILE_H
#define FILTERFILE_H

#include <QWidget>
#include <QSpinBox>

class FilterFile :
		public QWidget
{
	Q_OBJECT

public:
	explicit FilterFile(QWidget *parent, QString *path);
	bool toggleEQ(bool state);
	int filterDialog(QString type, QWidget *parent, QString msg = "", QString *objectName = 0); // 0=Cancel; 1=Ok,Save; 2=Discard
	bool getEQ(int *preamp, QString *activeConfig, QStringList *excludeConfigs);
	bool changeEQ(int preamp, QString *activeConfig);
	int saveEQ(QString *lastConfig, QString *newConfig); // 0=Cancel; 1=Save; 2=Discard
	bool removeEQ(QString *configName);
	void writeEQFile(QString *configName, QMap<int, QSpinBox *> *freqValues,
					 QMap<int, QDoubleSpinBox *> *levelValues);
	QMap<int, QList<double> > readEQFile(QString *configName);

private:
	QString version;
	QString *configPath;

	void initEQFile();
};

#endif // FILTERFILE_H
