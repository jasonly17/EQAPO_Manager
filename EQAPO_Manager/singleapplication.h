#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QObject>
#include <QGuiApplication>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QLocalServer>

class SingleApplication : public QGuiApplication
{
        Q_OBJECT
        Q_DISABLE_COPY(SingleApplication)
    public:
        enum Messages {
            BRINGTOFRONT
        };

        explicit SingleApplication(int& argc, char* argv[], const QString key);
        ~SingleApplication();

        bool isRunning() const;
        bool sendMessage(Messages msg);

    signals:
        void bringToFront();

    public slots:
        void receiveMessage();

    private:
        const QString m_uniqueKey;
        const QString m_memLockKey;

        bool m_isRunning;
        QSharedMemory m_sharedMem;
        QSystemSemaphore m_memLock;

        QLocalServer* m_localServer;

        void processMessage(Messages msg);
};

#endif // SINGLEAPPLICATION_H
