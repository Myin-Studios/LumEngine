#pragma once
#include <QHBoxLayout>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    QHBoxLayout* getMainLayout() const;

private:
    QWidget* m_CentralWidget = nullptr;
    QHBoxLayout* m_MainLayout = nullptr;
};
