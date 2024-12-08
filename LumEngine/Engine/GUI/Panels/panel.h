#pragma once

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QAbstractAnimation>

#include <iostream>

class BasePanel : public QWidget
{
public:
    BasePanel(std::string title = "");
    ~BasePanel();

private:
    std::unique_ptr<QFrame> _titleFrame;
    std::unique_ptr<QLabel> _title;
    std::unique_ptr<QFrame> _generalContainer;

    std::unique_ptr<QHBoxLayout> _titleLayout;
    std::unique_ptr<QVBoxLayout> _mainContainerLayout;
    std::unique_ptr<QVBoxLayout> _propertiesContainerLayout;
};

class Panel : public QFrame
{
    Q_OBJECT
public:
    Panel(QWidget *parent = nullptr);
    virtual ~Panel() = default;

    QVBoxLayout* getLayout() const { return m_layout; };

private:
    QVBoxLayout* m_layout = nullptr;
};

class VerticalPanel : public QFrame
{
    Q_OBJECT
public:
    VerticalPanel(QWidget* parent = nullptr) : QFrame(parent)
    {
        _layout = std::make_unique<QVBoxLayout>();
        _layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        setLayout(_layout.get());
        baseWidth = width();
    }

    QVBoxLayout* getLayout() const { return this->_layout.get(); };

    void setBaseWidth(int baseW)
    {
        this->baseWidth = baseW;
    }

private:
    bool toExpand = false;
    int baseWidth = 20;

    std::unique_ptr<QVBoxLayout> _layout;
};