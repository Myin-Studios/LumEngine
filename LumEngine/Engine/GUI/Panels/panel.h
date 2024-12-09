#pragma once

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QPainter>

#include <iostream>
#include <vector>

class PanelHeader : public QFrame
{
public:
    PanelHeader(const PanelHeader& other);
    PanelHeader(const std::string& title = "", QWidget* parent = nullptr);
    ~PanelHeader();

protected:
    void paintEvent(QPaintEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    std::unique_ptr<QString> _title;
};


class BasePanel : public QWidget
{
public:
    BasePanel(const std::string& title = "");
    ~BasePanel();

    void addHeader(const std::string& title);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<PanelHeader> _headers;
    std::unique_ptr<QFrame> _generalContainer;

    std::unique_ptr<QHBoxLayout> _headersLayout;
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