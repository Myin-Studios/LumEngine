#pragma once

#include <qframe.h>
#include <QHBoxLayout>

class Panel : public QFrame
{
    Q_OBJECT
public:
    Panel(QWidget *parent = 0);
    virtual ~Panel() = default;

    QHBoxLayout* getLayout() const { return m_layout; };

private:
    QHBoxLayout* m_layout = nullptr;
};
