#ifndef GUIHANDLER_H
#define GUIHANDLER_H

#include <QObject>

#include "equalizerfuncs.h"

class GuiHandler : public QObject
{
	Q_OBJECT
public:
	explicit GuiHandler(QQuickWindow *window, QObject *parent = 0);

	QObject *powerButton, *preampSlider, *comboBox, *bands;

	void load();

signals:
	void changeCombo();

public slots:
	void powerToggled(bool checked, bool reloadSettings = true);
	void preampChanged(double value);
	void comboBoxChanged(const QString &text);
	void resetButtonClicked();
	void saveButtonClicked();
	void deleteButtonClicked();
	void bandChanged(int type);

private:
	bool power, loaded, loading, sorting;
	enum BandValueType {Freq, Vol};
	QQuickWindow *window;
	EqualizerFuncs *eqFuncs;
	QString configPath, activeConfig, lastConfig;
	QStringList excludeConfigs, configList;
	double preamp;

	void loadEQFiles();
	void loadSettings(bool setIndex = false);
	QMap<int, QList<double> > getBandValues();
	QMap<int, QList<double> > sortBandValues(QMap<double, double> bandValues);
};

#endif // GUIHANDLER_H
