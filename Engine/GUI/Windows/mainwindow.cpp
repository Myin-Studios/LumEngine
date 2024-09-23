#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_CentralWidget = new QWidget(this);
    setCentralWidget(m_CentralWidget);

    m_MainLayout = new QHBoxLayout();
    m_CentralWidget->setLayout(m_MainLayout);

    m_CentralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MainWindow::~MainWindow()
{
    delete m_CentralWidget;
    delete m_MainLayout;
}

QHBoxLayout* MainWindow::getMainLayout() const
{
    return m_MainLayout;
}
