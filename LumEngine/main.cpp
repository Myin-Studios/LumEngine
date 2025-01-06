#include <string>

#include "Engine/GUI/Home/IHomeBuilder.h"

#include "QApplication"
#include <QOpenGLFramebufferObjectFormat>

#include <filesystem>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    std::unique_ptr<HomeBuilder> homeBuilder = std::make_unique<HomeBuilder>();

    return QApplication::exec();
}