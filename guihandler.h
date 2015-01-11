#ifndef GUIHANDLER_H
#define GUIHANDLER_H

#include <QObject>
#include <QStringListModel>
#include <QQuickWindow>

#include "equalizerfuncs.h"

class GuiHandler : public QObject
{
	Q_OBJECT
public:
	explicit GuiHandler(QQuickWindow *window, QObject *parent = 0);

	QObject *comboBox, *bands;

	void load();

signals:
	void changeCombo();

public slots:
	void powerToggled(bool checked);
	void preampChanged(double value);
	void comboBoxChanged(const QString &text);
	void resetButtonClicked();
	void saveButtonClicked();
	void deleteButtonClicked();
	void bandChanged();

private:
	bool loaded, loading, sorting;
	QQuickWindow *window;
	EqualizerFuncs *eqFuncs;
	QString configPath, activeConfig, lastConfig;
	QStringList excludeConfigs, configList;
	double preamp;

	void loadEQFiles();
	void loadSettings(bool setIndex = false);
	QMap<int, QList<double> > getBandValues();
};

#endif // GUIHANDLER_H
