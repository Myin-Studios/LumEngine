#include "panel.h"

Panel::Panel(QWidget* parent) : QFrame(parent)
{
    m_layout = new QVBoxLayout(this);

    setLayout(m_layout);
}

BasePanel::BasePanel(const std::string& title)
{
    this->_generalContainer = std::make_unique<QFrame>();
    this->_headersLayout = std::make_unique<QHBoxLayout>();
    this->_mainContainerLayout = std::make_unique<QVBoxLayout>();
    this->_headers.emplace_back(title);

    this->_generalContainer->setStyleSheet(
        "QFrame{"
        "border: none;"
        "border-top-right-radius: 10px;"
        "border-bottom-right-radius: 10px;"
        "border-bottom-left-radius: 10px;"
        "background-color: rgb(20, 20, 20);"
        "}"
    );

    this->_mainContainerLayout->setSpacing(0);
    this->_mainContainerLayout->addLayout(this->_headersLayout.get());
    this->_mainContainerLayout->addWidget(this->_generalContainer.get());

    this->_headersLayout->setContentsMargins({ 0, 0, 0, 0 });
    this->_headersLayout->setSpacing(2);

    for (auto& header : this->_headers)
    {
        this->_headersLayout->addWidget(&header);
        header.setMaximumHeight(30);
    }

    this->setLayout(this->_mainContainerLayout.get());
    this->setMinimumSize(300, this->height());
}

BasePanel::~BasePanel()
{
}

void BasePanel::addHeader(const std::string& title)
{
    this->_headers.emplace_back(title);
}

void BasePanel::paintEvent(QPaintEvent* event)
{
    for (auto& header : this->_headers)
    {
        this->_headersLayout->addWidget(&header);
        header.setMaximumHeight(30);
    }
}

PanelHeader::PanelHeader(const PanelHeader& other) : QFrame(other.parentWidget())
{
    if (other._title)
    {
        this->_title = std::make_unique<QString>(*other._title);
    }
}
PanelHeader::PanelHeader(const std::string& title, QWidget* parent) : QFrame(parent)
{
    this->_title = std::make_unique<QString>(QString(title.c_str()));
    
    this->setStyleSheet(
        "QFrame{"
        "border-top-left-radius: 10px;"
        "border-top-right-radius: 10px;"
        "background-color: rgb(20, 20, 20);"
        "}"
    );

    update();
}

PanelHeader::~PanelHeader()
{
}

void PanelHeader::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect title_rect(QPoint(0, 0), QSize(width(), height()));
    painter.fillRect(title_rect, Qt::transparent);
    painter.setPen(Qt::white);
    painter.drawText(title_rect, Qt::AlignCenter, *_title);

    this->setStyleSheet(
        "QFrame{"
        "border-top-left-radius: 10px;"
        "border-top-right-radius: 10px;"
        "background-color: rgb(20, 20, 20);"
        "}"
    );

    if (!hasFocus())
    {
        QLinearGradient gradient(0, 0, width(), 0);
        gradient.setColorAt(.3f, QColor(3, 102, 252, 0));   // Trasparente a sinistra
        gradient.setColorAt(0.5, QColor(150, 150, 150, 255)); // Colore pieno al centro
        gradient.setColorAt(.7f, QColor(3, 102, 252, 0));   // Trasparente a destra

        QPen pen;
        pen.setBrush(gradient);
        pen.setWidth(2);
        painter.setPen(pen);

        painter.drawLine(10, 0, width() - 10, 0);
    }
    else
    {
        QLinearGradient gradient(0, 0, width(), 0);
        gradient.setColorAt(.3f, QColor(3, 102, 252, 0));   // Trasparente a sinistra
        gradient.setColorAt(0.5, QColor(3, 102, 252, 255)); // Colore pieno al centro
        gradient.setColorAt(.7f, QColor(3, 102, 252, 0));   // Trasparente a destra

        QPen pen;
        pen.setBrush(gradient);
        pen.setWidth(2);
        painter.setPen(pen);

        painter.drawLine(10, 0, width() - 10, 0);
    }
}

void PanelHeader::focusInEvent(QFocusEvent* event)
{
    update();

    // this->setStyleSheet(
    //     "QFrame {"
    //     "    border-top-left-radius: 10px;"
    //     "    border-top-right-radius: 10px;"
    //     "    background-color: rgb(20, 20, 20);"
    //     "    border: none;"
    //     "    border-top: 2px solid linear-gradient(to right, "
    //     "                      rgba(3, 102, 252, 0), "
    //     "                      #0366fc, "
    //     "                      rgba(3, 102, 252, 0));"
    //     "    background-position: top;"
    //     "}"
    // );

}

void PanelHeader::focusOutEvent(QFocusEvent* event)
{
    update();

    // this->setStyleSheet(
    //     "QFrame{"
    //     "border: none;"
    //     "border-top: 2px solid rgb(150, 150, 150);"
    //     "border-top-left-radius: 10px;"
    //     "border-top-right-radius: 10px;"
    //     "background-color: rgb(20, 20, 20);"
    //     "}"
    // );
}

void PanelHeader::mousePressEvent(QMouseEvent* event)
{
    setFocus();
}
