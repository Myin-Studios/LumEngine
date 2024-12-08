#include "panel.h"

Panel::Panel(QWidget* parent) : QFrame(parent)
{
    m_layout = new QVBoxLayout(this);

    setLayout(m_layout);
}

BasePanel::BasePanel(std::string title)
{
    this->_title = std::make_unique<QLabel>();
    this->_titleFrame = std::make_unique<QFrame>();
    this->_generalContainer = std::make_unique<QFrame>();
    this->_titleLayout = std::make_unique<QHBoxLayout>();
    this->_mainContainerLayout = std::make_unique<QVBoxLayout>();

    this->_title->setText(QString(title.c_str()));

    this->_titleFrame->setFixedHeight(30);
    this->_titleFrame->setStyleSheet(
        "QFrame{"
        "border: none;"
        "border-top: 2px solid #00baff;"
        "border-top-left-radius: 10px;"
        "border-top-right-radius: 10px;"
        "background-color: rgb(20, 20, 20);"
        "}"
    );

    this->_titleFrame->setMaximumWidth(this->width() / 5);
    this->_titleFrame->setLayout(this->_titleLayout.get());

    this->_generalContainer->setStyleSheet(
        "QFrame{"
        "border: none;"
        "border-top-right-radius: 10px;"
        "border-bottom-right-radius: 10px;"
        "border-bottom-left-radius: 10px;"
        "background-color: rgb(20, 20, 20);"
        "}"
    );

    this->_titleLayout->addWidget(this->_title.get());
    this->_titleLayout->setSpacing(0);

    this->_mainContainerLayout->setSpacing(0);
    this->_mainContainerLayout->addWidget(this->_titleFrame.get(), Qt::AlignLeft);
    this->_mainContainerLayout->addWidget(this->_generalContainer.get());

    this->setLayout(this->_mainContainerLayout.get());
    this->setMinimumSize(300, this->height());
}

BasePanel::~BasePanel()
{
}
