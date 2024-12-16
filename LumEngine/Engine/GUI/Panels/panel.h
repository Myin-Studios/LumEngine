#pragma once

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QPainter>
#include <QPainterPath>
#include <QStackedWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>

#include "../Elements/Elements.h"

#include <iostream>
#include <vector>

class PanelHeader : public QPushButton
{
public:
    PanelHeader(const PanelHeader& other);
    PanelHeader(const std::string& title = "", QWidget* parent = nullptr);
    ~PanelHeader();

	std::unique_ptr<QString> getTitle() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
private:
    std::unique_ptr<QString> _title;
};

class ContainerPanel : public QFrame
{
public:
    ContainerPanel(QWidget* parent = nullptr);
    ~ContainerPanel();

    void addPage(BaseUIElement* elem, int id);
    const std::vector<UIElements>& getElements() const;

    QVBoxLayout* getLayout();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::unique_ptr<QVBoxLayout> _container;
    std::vector<UIElements> _elements;
};

class BasePanel : public QWidget
{
	Q_OBJECT
public:
    BasePanel(const std::string& title = "");
    ~BasePanel();

    void addHeader(const std::string& title);
    void addPage(const std::string& title, QWidget* elem);
    void addElement(const std::string& title, QWidget* elem);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<PanelHeader*> _headers;
    ContainerPanel* _generalContainer;
    QStackedWidget* _stackedWidget;

    QHBoxLayout* _headersLayout;
    std::vector<std::unique_ptr<QVBoxLayout>> _containerLayouts;
    QVBoxLayout* _mainContainerLayout;

private slots:
    void onHeaderClicked(int index);
};

class VPanel : public QFrame
{
    Q_OBJECT
public:
    VPanel(QWidget *parent = nullptr);
    virtual ~VPanel() = default;

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