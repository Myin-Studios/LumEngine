#include <string>

#include "QApplication"
#include "QCoreApplication"
#include "QStyleFactory"
#include "Engine/GUI/guibuilder.h"
#include "Engine/GUI/Loading/loadingWindow.h"
#include "Engine/Core/hostInitializer.h"
#include <QMetaObject>

#include <filesystem>

int main(int argc, char* argv[])
{
    SetEnvironmentVariable(L"COREHOST_TRACE", L"1");
    SetEnvironmentVariable(L"COREHOST_TRACEFILE", L"corehost.log");
    SetEnvironmentVariable(L"COREHOST_TRACE_VERBOSITY", L"4");

    QApplication a(argc, argv);

    auto loadingWindow = new LoadingWindow();
    loadingWindow->show();

    GuiBuilder* builder = nullptr;
    LoadingThread* loadingThread = new LoadingThread();

    loadingWindow->setThread(loadingThread);

    // Collega il segnale di aggiornamento del progresso alla finestra di caricamento
    QObject::connect(loadingThread, &LoadingThread::progressUpdated, loadingWindow, &LoadingWindow::setProgress);

    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    loadingWindow->setProgress("Init GUI...", 100);

    // Usa QMetaObject::invokeMethod per creare GuiBuilder nel thread principale
    QObject::connect(loadingThread, &LoadingThread::finished, [&]() {
        QMetaObject::invokeMethod(qApp, [&]() {
            loadingWindow->close();
            builder = new GuiBuilder();

            builder->getPlayButton()->SetScriptRunner(loadingThread->getStart(), loadingThread->getUpdate());
            builder->getMainWindow()->setThread(loadingThread);

#if defined(Q_OS_WIN)
            QApplication::setStyle(QStyleFactory::create("Fusion")); // Tema Fusion per Windows
            QIcon icon(":/Icons/CMEngine_Logo.ico");
            if (!icon.isNull()) {
                builder->getMainWindow()->setWindowIcon(icon);
            }
#elif defined(Q_OS_LINUX)
            if (QSysInfo::productVersion().startsWith("5")) {
                QApplication::setStyle(QStyleFactory::create("Fusion")); // Cambia con "Plasma" se supportato
            }

            if (QIcon icon(":/Icons/CMEngine_Logo.svg"); icon.isNull()) {
                qWarning() << "Icon not found!";
            } else {
                QApplication::setWindowIcon(icon);
                builder->getMainWindow()->setWindowIcon(icon);
            }
#endif
        }, Qt::QueuedConnection);  // Qt::QueuedConnection assicura che venga eseguito nel thread della GUI
    });

    loadingThread->start();

    return QApplication::exec();
}