/**********************************************************************
** Copyright (C) 2000-2002 Trolltech AS.  All rights reserved.
**
** This file is part of the Qtopia Environment.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/
#ifndef DESKTOP_H
#define DESKTOP_H

#include <qwidget.h>
#include <qdatetime.h>

//#include "qcopbridge.h"

class QCopBridge;
class QHostAddress;
class TransferServer;
class PackageHandler;
class ServiceRequest;
class TempScreenSaverMonitor;
class AppLauncher;
class AppLnkSet;
class StorageInfo;
class SyncDialog;
class DocumentList;
class ServerInterface;
class QueuedRequestRunner;
namespace Opie {
namespace Core {
    class ODeviceButton;
    class OProcess;
}
}

class Server : public QWidget {
    Q_OBJECT
public:
    Server();
    ~Server();

    static bool mkdir(const QString &path);

    void show();

    static bool setKeyboardLayout( const QString &kb );

public slots:
    void systemMsg(const QCString &, const QByteArray &);
    void receiveTaskBar(const QCString &msg, const QByteArray &data);
    void terminateServers();
    void pokeTimeMonitors();

private slots:
    //@{
    void startSoundServer();
    void soundServerExited();
    void startBluetoothServer();
    //@}
    void activate(const Opie::Core::ODeviceButton*,bool);
    void syncConnectionClosed( const QHostAddress & );
    void applicationLaunched(int pid, const QString &app);
    void applicationTerminated(int pid, const QString &app);
    void applicationConnected(const QString &app);
    void storageChanged();
    void cancelSync();
    void desktopMessage( const QCString &, const QByteArray & );
    void runDirectAccess();
    void finishedQueuedRequests();

protected:
    void styleChange( QStyle & );
    void timerEvent( QTimerEvent *e );

private:
    void layout();
    void startTransferServer();
    void preloadApps();
    void prepareDirectAccess();
    void postDirectAccess();
    QString cardInfoString();
    QString installLocationsString();
    void setLed(int led, int state);
    QString getNotifyLed();

    QCopBridge *qcopBridge;
    TransferServer *transferServer;
    PackageHandler *packageHandler;
    QDate last_today_show;
    int tid_xfer;
    /* ### FIXME two below### */
//    int tid_today;
//    TempScreenSaverMonitor *tsmMonitor;
    StorageInfo *storage;
    SyncDialog *syncDialog;
    AppLauncher *appLauncher;
    DocumentList *docList;
    ServerInterface *serverGui;

    int pendingFlushes;
    bool directAccessRun;
    QueuedRequestRunner *qrr;

    Opie::Core::OProcess *process;
};


#endif // DESKTOP_H

