#pragma once

#include <QFrame>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QAbstractAnimation>

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