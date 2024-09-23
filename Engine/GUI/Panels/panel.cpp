#include "panel.h"

Panel::Panel(QWidget* parent) : QFrame(parent)
{
    m_layout = new QHBoxLayout(this);

    setLayout(m_layout);
}