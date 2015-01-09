#ifndef GUIHANDLER_H
#define GUIHANDLER_H

#include <QObject>
#include <QStringListModel>
#include <QQuickView>

class GuiHandler : public QObject
{
	Q_OBJECT
public:
	explicit GuiHandler(QQuickView *view, QObject *parent = 0);

	QObject *comboBox, *bandsGroup;

	void load();

signals:
	void changeCombo();

public slots:
	void on_powerToggled(bool checked);
	void on_preampChanged(double value);
	void on_comboChanged(const QString &text);
	void on_resetButtonClicked();
	void on_saveButtonClicked();
	void on_deleteButtonClicked();
	void on_bandVolumeChanged(int index, double value);
	void on_bandFreqChanged(int index, double value);

private:
	bool loaded, loading, sorting;
	QQuickView *view;
	QString configPath, activeConfig;
	QStringList excludeConfigs, configList;
	int preamp;

	void loadEQFiles();
	void loadSettings(bool setIndex = false);
	void swapBand(int oldBandNum, int newBandNum);
};

#endif // GUIHANDLER_H
