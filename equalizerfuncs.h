#ifndef EQUALIZERFUNCS_H
#define EQUALIZERFUNCS_H

#include <QObject>
#include <QQuickWindow>
#include <QQmlEngine>

class EqualizerFuncs : public QObject
{
	Q_OBJECT
public:
	explicit EqualizerFuncs(QString *configPath, QObject *parent = 0);

	enum DialogType {MessageDialog, SaveDialog, OverwriteDialog, DeleteDialog};

	void toggleEQ(bool state);
	void displayDialog(int type, QQuickWindow *window, QString configName, QString lastConfig = "");
	bool getConfig(double *preamp, QString *activeConfig, QStringList *excludeConfigs);
	bool changeConfig(double preamp, QString activeConfig);
	QMap<int, QList<double> > readEQFile(QString configName);
	void writeEQFile(QString configName, QMap<int, QList<double> > *settings);

signals:
	void result(QString text);

public slots:
	void okButtonClicked(int type, QString textInput = "", QString lastConfig = "");
	void discardButtonClicked();
	void cancelButtonClicked(int type);

private:
	QString version;
	QRegExp invalidChars;
	QString configPath;
	QQuickWindow *dialog, *subDialog;

	void initEQFile();
};

#endif // EQUALIZERFUNCS_H
