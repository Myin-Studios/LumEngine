#include "panel.h"

VPanel::VPanel(QWidget* parent) : QFrame(parent)
{
    m_layout = new QVBoxLayout(this);

    setLayout(m_layout);
}

BasePanel::BasePanel(const std::string& title)
{
    // Layouts per la struttura principale
    this->_headersLayout = new QHBoxLayout();
    this->_mainContainerLayout = new QVBoxLayout();
    this->_stackedWidget = new QStackedWidget();
    this->_searchField = new TextField();

    // Assicurati che il QStackedWidget possa espandersi
    this->_stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->_stackedWidget->setContentsMargins(0, 0, 0, 0);

    this->_searchField->setPlaceholderText("Search...");
    this->_searchField->setSizeIncrement(0, 0);
    this->_searchField->setMaximumHeight(30);
    this->_searchField->setVisible(false);

    // Imposta il layout principale
    this->_mainContainerLayout->setContentsMargins(10, 0, 10, 0);
    this->_mainContainerLayout->setSpacing(10);
    this->_mainContainerLayout->addLayout(this->_headersLayout);
    this->_mainContainerLayout->addWidget(_searchField);
    this->_mainContainerLayout->addWidget(this->_stackedWidget);
    this->_mainContainerLayout->addStretch();

    this->_headersLayout->setContentsMargins(0, 0, 0, 0);
    this->_headersLayout->setSpacing(2);

    // Configura il layout del pannello
    this->setLayout(this->_mainContainerLayout);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->setMinimumSize(300, 0);
}

BasePanel::~BasePanel()
{
}

void BasePanel::addHeader(const std::string& title)
{
    auto* _header = new PanelHeader(title);
    _headers.append(_header);

    connect(_header, &PanelHeader::clicked, this, [this, _header]() {
        int index = _headers.indexOf(_header);
        if (index != -1) {
            _header->setFocus();
            onHeaderClicked(index);
            this->update();
        }
        });

    _headersLayout->addWidget(_header);

    // Crea una nuova QScrollArea per la pagina
    QScrollArea* scrollable = new QScrollArea();
    QWidget* scrollContent = new QWidget();
    QVBoxLayout* scrollContentLayout = new QVBoxLayout(scrollContent);
    QScrollBar* scrollBar = new QScrollBar();

    // Configura lo scroll area
    scrollable->setWidget(scrollContent);
    scrollable->setWidgetResizable(true); // Permette il ridimensionamento
    scrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    scrollBar->setStyleSheet(
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: rgb(25, 25, 25);"
        "    width: 10px;" // Questo controlla la larghezza del contenitore
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: rgb(50, 50, 50);"
        "    min-height: 20px;"
        "    border-radius: 3px;"
        "    width: 5px;" // Larghezza iniziale del gestore
        "    margin-left: 2px;" // Centrare il gestore
        "    margin-right: 2px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: rgb(70, 70, 70);"
        "    width: 10px;" // Larghezza al passaggio del mouse
        "    margin-left: 1px;" // Regola per centrare
        "    margin-right: 1px;"
        "}"
        "QScrollBar::add-line:vertical {"
        "    height: 0px;"
        "    subcontrol-position: bottom;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "    subcontrol-position: top;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
        "    background: none;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}"
    );

    scrollable->setVerticalScrollBar(scrollBar);

    scrollContent->setLayout(scrollContentLayout);

    // Imposta le politiche di dimensionamento
    scrollable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollable->setWidget(scrollContent);
    scrollable->setWidgetResizable(true); // Permette il ridimensionamento
    scrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Aggiungi la QScrollArea al QStackedWidget
    _stackedWidget->addWidget(scrollable);
}

void BasePanel::addPage(const std::string& title, QWidget* elem)
{
    // Crea un header per la nuova pagina
    auto* _header = new PanelHeader(title);
    _headers.append(_header);

    // Connetti il segnale di click dell'header a uno slot
    connect(_header, &PanelHeader::clicked, this, [this, _header]() {
        int index = _headers.indexOf(_header);
        if (index != -1) {
            _header->setFocus();
            onHeaderClicked(index);
            this->update();
        }
        });

    // Aggiungi l'header al layout degli header
    _headersLayout->addWidget(_header);

    // Crea una nuova QScrollArea per la pagina
    QScrollArea* scrollable = new QScrollArea();
    QWidget* scrollContent = new QWidget();
    QVBoxLayout* scrollContentLayout = new QVBoxLayout(scrollContent);
    QScrollBar* scrollBar = new QScrollBar();

    // Configura lo scroll area
    scrollable->setWidget(scrollContent);
    scrollable->setWidgetResizable(true); // Permette il ridimensionamento
    scrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    scrollBar->setStyleSheet(
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: rgb(25, 25, 25);"
        "    width: 10px;" // Questo controlla la larghezza del contenitore
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: rgb(50, 50, 50);"
        "    min-height: 20px;"
        "    border-radius: 3px;"
        "    width: 5px;" // Larghezza iniziale del gestore
        "    margin-left: 2px;" // Centrare il gestore
        "    margin-right: 2px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: rgb(70, 70, 70);"
        "    width: 10px;" // Larghezza al passaggio del mouse
        "    margin-left: 1px;" // Regola per centrare
        "    margin-right: 1px;"
        "}"
        "QScrollBar::add-line:vertical {"
        "    height: 0px;"
        "    subcontrol-position: bottom;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "    subcontrol-position: top;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
        "    background: none;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}"
    );
    
    scrollable->setVerticalScrollBar(scrollBar);

    scrollContent->setLayout(scrollContentLayout);

    // Imposta le politiche di dimensionamento
    scrollable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollable->setWidget(scrollContent);
    scrollable->setWidgetResizable(true); // Permette il ridimensionamento
    scrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Aggiungi la QScrollArea al QStackedWidget
    _stackedWidget->addWidget(scrollable);

    // Aggiungi l'elemento al layout della nuova pagina
    scrollContentLayout->addWidget(elem);
	scrollContentLayout->setContentsMargins(0, 0, 0, 0);
    scrollContentLayout->addSpacing(10);

    // Se è il primo header, selezionalo
    if (_headers.size() == 1) {
        onHeaderClicked(0);
    }
}

void BasePanel::addElement(const std::string& title, QWidget* elem) {
    auto it = std::find_if(_headers.begin(), _headers.end(), [&title](PanelHeader* header) {
        return header->getTitle()->toStdString() == title;
        });

    if (it != _headers.end()) {
        this->_searchField->setVisible(true);
        int pageIndex = _headers.indexOf(*it);
        QScrollArea* scrollable = qobject_cast<QScrollArea*>(_stackedWidget->widget(pageIndex));
        if (scrollable) {
            QWidget* scrollContent = scrollable->widget();
            QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollContent->layout());
            if (layout) {
                // Controlla se esiste già un PropertyGroup con lo stesso titolo
                PropertyGroup* newGroup = qobject_cast<PropertyGroup*>(elem);
                if (newGroup) {
                    PropertyGroup* newProperty = dynamic_cast<PropertyGroup*>(newGroup);
                    for (int i = 0; i < layout->count(); ++i) {
                        PropertyGroup* existingGroup = qobject_cast<PropertyGroup*>(layout->itemAt(i)->widget());
                        PropertyGroup* existingProperty = dynamic_cast<PropertyGroup*>(existingGroup);

                        if (existingProperty && newProperty) {
                            bool sameTitle = existingGroup->getTitle() == newGroup->getTitle();
                            bool sameEntity = existingProperty->GetEntity() == newProperty->GetEntity();

                            qDebug() << "Comparing properties:";
                            qDebug() << "Same title:" << sameTitle;
                            qDebug() << "Same entity:" << sameEntity;
                            qDebug() << "Existing entity ID:" << (existingProperty->GetEntity() ? existingProperty->GetEntity()->GetEntityID() : -1);
                            qDebug() << "New entity ID:" << (newProperty->GetEntity() ? newProperty->GetEntity()->GetEntityID() : -1);

                            if (sameTitle && !sameEntity) {
                                layout->removeWidget(existingGroup);
                                existingGroup->deleteLater();
                                layout->addWidget(elem);
                                return;
                            }
                            else if (sameTitle && sameEntity) {
                                existingProperty->UpdateValues();
                                delete elem;
                                return;
                            }
                        }
                    }
                }

                layout->addWidget(elem);
                return;
            }
        }
    }
    else {
        addPage(title, elem);
    }
}

void BasePanel::removeElement(const std::string& title, const std::string& elemName)
{
    auto it = std::find_if(_headers.begin(), _headers.end(), [&title](PanelHeader* header) {
        return header->getTitle()->toStdString() == title;
        });

    if (it != _headers.end()) {
        int pageIndex = _headers.indexOf(*it);
        QScrollArea* scrollable = qobject_cast<QScrollArea*>(_stackedWidget->widget(pageIndex));
        if (scrollable) {
            QWidget* scrollContent = scrollable->widget();
            QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollContent->layout());

            if (layout) {
                for (int i = 0; i < layout->count(); ++i) {
                    PropertyGroup* group = qobject_cast<PropertyGroup*>(layout->itemAt(i)->widget());
                    if (group) {
                        // Debug: stampa i titoli per confronto
                        qDebug() << "Comparing titles:";
                        qDebug() << "Group title:" << group->getTitle();
                        qDebug() << "Expected title:" << QString::fromStdString(elemName);

                        if (group->getTitle() == elemName) {
                            group->hide();
                            layout->removeWidget(group);
                            layout->update();
                            scrollContent->updateGeometry();
                            scrollable->updateGeometry();
                            return;
                        }
                    }
                }
            }
        }
    }
}

void BasePanel::removeAllElements(const std::string& title)
{
    auto it = std::find_if(_headers.begin(), _headers.end(), [&title](PanelHeader* header) {
        return header->getTitle()->toStdString() == title;
        });

    if (it != _headers.end()) {
        this->_searchField->setVisible(false);
        int pageIndex = _headers.indexOf(*it);
        QScrollArea* scrollable = qobject_cast<QScrollArea*>(_stackedWidget->widget(pageIndex));
        if (scrollable) {
            QWidget* scrollContent = scrollable->widget();
            QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollContent->layout());
            if (layout) {
                // Itera attraverso tutti gli elementi del layout
                for (int i = layout->count() - 1; i >= 0; --i) {
                    QWidget* widget = layout->itemAt(i)->widget();
                    if (widget) {
                        layout->removeWidget(widget);
                        widget->deleteLater();
                    }
                }
                // Aggiorna il layout per riflettere i cambiamenti
                layout->update();
                scrollContent->updateGeometry();
            }
        }
    }
}

void BasePanel::paintEvent(QPaintEvent* event)
{
    // Esegui qualcosa per ogni header (se necessario)
	for (auto& _header : _headers) {
        if (!_header->hasFocus())
        {
            _header->setStyleSheet(
                "QPushButton {"
                "border-top-left-radius: 10px;"
                "border-top-right-radius: 10px;"
                "background-color: rgb(25, 25, 25);"
                "color: gray;"
                "}"
            );
        }
	}

    // Una volta finito il ciclo, imposta l'indice corrente a -1
    this->_stackedWidget->setCurrentIndex(-1);  // Nasconde tutti i widget
}

void BasePanel::onHeaderClicked(int index)
{
    _stackedWidget->setCurrentIndex(index);

    // Aggiorna lo stile degli header
    for (int i = 0; i < _headers.size(); ++i) {
        if (i == index) {
            _headers[i]->setStyleSheet(
                "QPushButton {"
                "border-top-left-radius: 10px;"
                "border-top-right-radius: 10px;"
                "background-color: rgb(3, 102, 252);"
                "color: white;"
                "}"
            );
        }
        else {
            _headers[i]->setStyleSheet(
                "QPushButton {"
                "border-top-left-radius: 10px;"
                "border-top-right-radius: 10px;"
                "background-color: rgb(25, 25, 25);"
                "color: gray;"
                "}"
            );
        }
    }
}

PanelHeader::PanelHeader(const PanelHeader& other) : QPushButton("", other.parentWidget())
{
    if (other._title)
    {
        this->_title = std::make_unique<QString>(*other._title);
    }
}

PanelHeader::PanelHeader(const std::string& title, QWidget* parent) : QPushButton("", parent)
{
    this->_title = std::make_unique<QString>(QString(title.c_str()));

    this->setStyleSheet(
        "QPushButton {"
        "border-top-left-radius: 10px;"
        "border-top-right-radius: 10px;"
        "background-color: rgb(25, 25, 25);"
        "border: none;"
        "}"
    );

    setFixedHeight(30);
    update();
}

PanelHeader::~PanelHeader()
{
}

std::unique_ptr<QString> PanelHeader::getTitle() const
{
	return std::make_unique<QString>(*_title);
}

void PanelHeader::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect title_rect(0, 0, width(), height());

    // Creiamo un percorso con bordi arrotondati solo in alto
    QPainterPath path;
    path.moveTo(10, 0); // Inizia dal punto superiore sinistro
    path.lineTo(width() - 10, 0); // Linea orizzontale sopra
    path.quadTo(width(), 0, width(), 10); // Curva verso l'angolo in alto a destra
    path.lineTo(width(), height() - 10); // Linea verticale destra
    path.quadTo(width(), height(), width(), height()); // Curva in basso a destra
    path.lineTo(10, height()); // Linea verticale sinistra
    path.quadTo(0, height(), 0, height()); // Curva in basso a sinistra
    path.lineTo(0, 10); // Linea orizzontale a sinistra
    path.quadTo(0, 0, 10, 0); // Curva in alto a sinistra
    path.closeSubpath(); // Chiudi il percorso

    if (!hasFocus()) {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QColor(25, 25, 25)); // Colore di sfondo
        painter.fillPath(path, painter.brush());
        painter.setPen(Qt::gray);
        painter.drawText(title_rect, Qt::AlignCenter, *_title);

        // Definiamo il gradiente
        QLinearGradient gradient(0, 0, width(), 0);
        gradient.setColorAt(0.3f, QColor(3, 102, 252, 0));
        gradient.setColorAt(0.5, QColor(150, 150, 150, 255));
        gradient.setColorAt(0.7f, QColor(3, 102, 252, 0));
        QPen pen;
        pen.setBrush(gradient);
        pen.setWidth(2);
        painter.setPen(pen);

        painter.drawLine(10, 0, width() - 10, 0);
    }
    else {
        painter.setRenderHint(QPainter::Antialiasing); // Abilita l'antialiasing
        painter.setBrush(QColor(30, 30, 30));
        painter.fillPath(path, painter.brush()); // Riempie il percorso con il colore di sfondo
        painter.setPen(Qt::white);
        painter.drawText(title_rect, Qt::AlignCenter, *_title);

        // Definiamo il gradiente
        QLinearGradient gradient(0, 0, width(), 0);
        gradient.setColorAt(0.1f, QColor(3, 102, 252, 0));
        gradient.setColorAt(0.5, QColor(3, 102, 252, 255));
        gradient.setColorAt(0.9f, QColor(3, 102, 252, 0));
        QPen pen;
        pen.setBrush(gradient);
        pen.setWidth(5);
        painter.setPen(pen);

        painter.drawLine(10, 0, width() - 10, 0);
    }
}


void PanelHeader::focusInEvent(QFocusEvent* event)
{
    update();
}

void PanelHeader::focusOutEvent(QFocusEvent* event)
{
    update();
}

ContainerPanel::ContainerPanel(QWidget* parent)
    : QFrame(parent)
{
    // Creiamo il layout iniziale
    this->_container = std::make_unique<QVBoxLayout>();
    this->setLayout(this->_container.get());  // Associa il layout a ContainerPanel
}

ContainerPanel::~ContainerPanel()
{
}

void ContainerPanel::addPage(BaseUIElement* elem, int id)
{
    if (!elem)
    {
        qDebug() << "Elemento nullo passato a addElement";
        return;
    }

    // Controlla se ID è valido per l'aggiunta
    if (id >= 0 && id < this->_elements.size())
    {
        this->_elements.at(id).addPage(elem);
    }
    else
    {
        // Aggiungi un nuovo gruppo di elementi se necessario
        UIElements e;
        e.addPage(std::move(elem));
        this->_elements.push_back(std::move(e));
    }

    // Aggiungi l'elemento al layout
    if (this->_container)
    {
        this->_container->addWidget(elem);
    }
    else
    {
        qDebug() << "Il container è nullo in ContainerPanel::addElement()";
    }
}

const std::vector<UIElements>& ContainerPanel::getElements() const {
    return this->_elements;
}

QVBoxLayout* ContainerPanel::getLayout()
{
    return this->_container.get();
}

void ContainerPanel::paintEvent(QPaintEvent* event)
{
    setStyleSheet(
        "QFrame{"
        "border: none;"
        "border-top-right-radius: 10px;"
        "border-bottom-right-radius: 10px;"
        "border-bottom-left-radius: 10px;"
        "background-color: rgb(25, 25, 25);"
        "}"
    );
}
