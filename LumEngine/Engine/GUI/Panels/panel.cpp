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

    // Creazione dello scroll area
    QScrollArea* scrollable = new QScrollArea();
    QWidget* scrollContent = new QWidget();
    QVBoxLayout* scrollContentLayout = new QVBoxLayout(scrollContent);

    // Configura lo scroll area
    scrollable->setWidget(scrollContent);
    scrollable->setWidgetResizable(true); // Permette il ridimensionamento
    scrollable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Configurazione del layout interno
    scrollContentLayout->addWidget(this->_stackedWidget);
    scrollContent->setLayout(scrollContentLayout);

    // Imposta il layout principale
    this->_mainContainerLayout->setSpacing(0);
    this->_mainContainerLayout->addLayout(this->_headersLayout);
    this->_mainContainerLayout->addWidget(scrollable);

    this->_headersLayout->setContentsMargins(10, 10, 10, 10);
    this->_headersLayout->setSpacing(2);

    // Configura il layout del pannello
    this->setLayout(this->_mainContainerLayout);
    this->setMinimumSize(300, this->height());
}

BasePanel::~BasePanel()
{
}

void BasePanel::addHeader(const std::string& title)
{
    // this->_headers.emplace_back(title);
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

    // Aggiungi il widget al QStackedWidget
    _stackedWidget->addWidget(elem);

    // Se è il primo header, selezionalo
    if (_headers.size() == 1) {
        onHeaderClicked(0);
    }
}

void BasePanel::addElement(const std::string& title, QWidget* elem)
{
    for (const auto& _header : _headers) {
        if (_header->getTitle()->toStdString() == title) {
            // Trova l'indice della pagina esistente
            int pageIndex = _headers.indexOf(_header);
            QWidget* page = _stackedWidget->widget(pageIndex);

            if (page) {
                // Aggiungi il nuovo elemento al layout della pagina
                QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(page->layout());
                if (layout) {
                    layout->addWidget(elem);
                    return;
                }
                else {
                    qDebug() << "Errore: la pagina esistente non ha un layout valido!";
                    return;
                }
            }
        }
    }

    // Se la pagina non esiste, crea una nuova pagina e aggiungila
    addPage(title, elem);
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

    // Impostiamo lo stile con bordi arrotondati
    this->setStyleSheet(
        "QPushButton {"
        "border-top-left-radius: 10px;"
        "border-top-right-radius: 10px;"
        "background-color: rgb(25, 25, 25);"
        "border: none;" // Rimuove i bordi predefiniti del QPushButton
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
