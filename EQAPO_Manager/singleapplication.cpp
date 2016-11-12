#include "singleapplication.h"
#include <QCryptographicHash>
#include <QLocalSocket>

namespace
{
    QString generateKeyHash(const QString& key, const QString& id)
    {
        QByteArray data;
        data.append(key.toUtf8()).append(id.toUtf8());

        return QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
    }
}

SingleApplication::SingleApplication(int &argc, char *argv[], const QString key)
    : QGuiApplication(argc, argv),
      m_isRunning(false),
      m_uniqueKey(generateKeyHash(key, "_sharedMemKey")),
      m_memLockKey(generateKeyHash(key, "_memLockKey")),
      m_sharedMem(m_uniqueKey),
      m_memLock(m_memLockKey, 1)
{
    // check if the current instance is attached to the shared memory
    if (m_sharedMem.isAttached())
    {
        m_isRunning = false;
        return;
    }

    // check if the current instance can attach to the shared memory
    m_memLock.acquire();
    if (m_sharedMem.attach())
    {
        m_isRunning = true;
        m_sharedMem.detach();
    }
    m_memLock.release();

    // failed to attach to the shared memory
    if (!isRunning())
    {
        // see if we can create the shared memory
        m_memLock.acquire();
        if (!m_sharedMem.create(1))
        {
            m_isRunning = true;
        }
        m_memLock.release();

        if (!isRunning())
        {
            // create a local server to listen for messages from other instances
            m_localServer = new QLocalServer(this);
            connect(m_localServer, &QLocalServer::newConnection,
                    this, &SingleApplication::receiveMessage);
            m_localServer->listen(m_uniqueKey);
        }
    }
}

SingleApplication::~SingleApplication()
{
    m_memLock.acquire();
    if (m_sharedMem.isAttached())
    {
        m_sharedMem.detach();
    }
    m_memLock.release();

    m_localServer->deleteLater();
}

// Returns true if an instance of the application is already running.
bool SingleApplication::isRunning() const
{
    return m_isRunning;
}

bool SingleApplication::sendMessage(Messages msg)
{
    // make sure there is another instance running
    if (!m_isRunning)
    {
        return false;
    }

    // send to message
    QLocalSocket localSocket(this);
    localSocket.connectToServer(m_uniqueKey, QIODevice::WriteOnly);
    if (!localSocket.waitForConnected(1000))
    {
        qDebug() << localSocket.errorString().toUtf8();
        return false;
    }

    localSocket.write(QString::number(msg).toUtf8());
    if (!localSocket.waitForBytesWritten(1000))
    {
        qDebug() << localSocket.errorString().toUtf8();
        return false;
    }

    localSocket.disconnectFromServer();
    return true;
}

void SingleApplication::receiveMessage()
{
    // read the incoming message
    QLocalSocket* localSocket = m_localServer->nextPendingConnection();
    if (!localSocket->waitForReadyRead(1000))
    {
        qDebug() << localSocket->errorString().toUtf8();
        return;
    }

    QByteArray data = localSocket->readAll();
    Messages msg = static_cast<Messages>(QString::fromUtf8(data.constData()).toInt());
    processMessage(msg);
}

void SingleApplication::processMessage(Messages msg)
{
    if (msg == Messages::BRINGTOFRONT)
    {
        emit bringToFront();
    }
}
