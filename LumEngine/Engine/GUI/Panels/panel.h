#pragma once

#include <qframe.h>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QAbstractAnimation>

class Panel : public QFrame
{
    Q_OBJECT
public:
    Panel(QWidget *parent = nullptr);
    virtual ~Panel() = default;

    QHBoxLayout* getLayout() const { return m_layout; };

private:
    QHBoxLayout* m_layout = nullptr;
};

class VerticalPanel : public Panel
{
    Q_OBJECT
public:
    VerticalPanel(QWidget* parent = nullptr) : Panel(parent)
    {

    }

    void setBaseWidth(int baseW)
    {
        this->baseWidth = baseW;
    }

public slots:
    void expand()
    {
        toExpand = !toExpand;

        QPropertyAnimation anim(this, "geometry");

        anim.setDuration(500);
        anim.setStartValue(QRect(x(), y(), 20, height()));
        anim.setEndValue(QRect(x(), y(), 300, height()));

        anim.start();
    }

private:
    bool toExpand = false;
    int baseWidth = 20;
};