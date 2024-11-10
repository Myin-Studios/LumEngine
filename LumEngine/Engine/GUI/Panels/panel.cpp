#include "panel.h"

Panel::Panel(QWidget* parent) : QFrame(parent)
{
    m_layout = new QVBoxLayout(this);

    setLayout(m_layout);
}