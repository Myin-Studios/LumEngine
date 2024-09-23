#include <QApplication>
#include <QPushButton>
#include <QStyleFactory>
#include <QSysInfo>
#include "Engine/GUI/guibuilder.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);

    auto builder = new GuiBuilder();

#if defined(Q_OS_WIN)
    QApplication::setStyle(QStyleFactory::create("Fusion")); // Tema Fusion per Windows
    QIcon icon(":/Icons/CMEngine_Logo.ico");
    if (!icon.isNull()) {
        builder->getMainWindow()->setWindowIcon(icon);
    }

#elif defined(Q_OS_LINUX)
    if (QSysInfo::productVersion().startsWith("5")) {
        // Imposta il tema Plasma se disponibile su Linux
        QApplication::setStyle(QStyleFactory::create("Fusion")); // Cambia con "Plasma" se supportato
    }

    if (QIcon icon(":/Icons/CMEngine_Logo.svg"); icon.isNull()) {
        qWarning() << "Icon not found!";
    } else {
        QApplication::setWindowIcon(icon);
        builder->getMainWindow()->setWindowIcon(icon);
    }
#endif


    // QPushButton button("Hello world!", nullptr);
    // button.resize(200, 100);
    // button.show();
    return QApplication::exec();
}
