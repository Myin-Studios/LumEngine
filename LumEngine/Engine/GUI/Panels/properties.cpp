#include "properties.h"

Vec3Property::Vec3Property(QWidget* parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->setMinimumSize(0, 0);
    this->setStyleSheet(
        "background-color: rgb(30, 30, 30);"
    );

    xValue = new NumberOperatorLineEdit(this);
    yValue = new NumberOperatorLineEdit(this);
    zValue = new NumberOperatorLineEdit(this);

    xValue->setCoordinate(Coordinate::X);
    yValue->setCoordinate(Coordinate::Y);
    zValue->setCoordinate(Coordinate::Z);

    xValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    yValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    zValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    _mainLayout = new QHBoxLayout(parent);

    _mainLayout->addWidget(xValue);
    _mainLayout->addWidget(yValue);
    _mainLayout->addWidget(zValue);
    
    _mainLayout->setContentsMargins(0, 0, 0, 0);

    xValue->show();
    yValue->show();
    zValue->show();

    setLayout(_mainLayout);
}

Vec3Property::~Vec3Property()
{
    delete xValue;
    delete yValue;
    delete zValue;
    delete xLabel;
    delete yLabel;
    delete zLabel;
    delete xTextFrame;
    delete yTextFrame;
    delete zTextFrame;
    delete _mainLayout;
}

CoordinateFrame::CoordinateFrame(std::string text, QWidget* parent) : QFrame(parent)
{
	this->_text = text;
}

CoordinateFrame::~CoordinateFrame()
{
}

void CoordinateFrame::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
    QRect rect(0, 0, width(), height());
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(255, 255, 255), 1));

    painter.drawText(rect, Qt::AlignCenter, QString(this->_text.c_str()));
}

NumberOperatorLineEdit::NumberOperatorLineEdit(QWidget* parent) : QLineEdit(parent) {
    normalStyleX = "QLineEdit {"
        "background-color: rgb(25, 25, 25);"
        "border: 2 solid rgb(127, 0, 0);"
        "border-radius: 7px;"
        "color: rgb(255, 255, 255);"
        "}";

    focusStyleX = "QLineEdit {"
        "background-color: rgb(25, 25, 25);"
        "border: 2 solid rgb(255, 0, 0);"
        "border-radius: 7px;"
        "color: rgb(255, 255, 255);"
        "}";

    normalStyleY = "QLineEdit {"
        "background-color: rgb(25, 25, 25);"
        "border: 2 solid rgb(0, 127, 0);"
        "border-radius: 7px;"
        "color: rgb(255, 255, 255);"
        "}";

    focusStyleY = "QLineEdit {"
        "background-color: rgb(25, 25, 25);"
        "border: 2 solid rgb(0, 255, 0);"
        "border-radius: 7px;"
        "color: rgb(255, 255, 255);"
        "}";

    normalStyleZ = "QLineEdit {"
        "background-color: rgb(25, 25, 25);"
        "border: 2 solid rgb(0, 0, 127);"
        "border-radius: 7px;"
        "color: rgb(255, 255, 255);"
        "}";

    focusStyleZ = "QLineEdit {"
        "background-color: rgb(25, 25, 25);"
        "border: 2 solid rgb(0, 0, 255);"
        "border-radius: 7px;"
        "color: rgb(255, 255, 255);"
        "}";

    normalStyleW = "QLineEdit {"
        "background-color: rgb(25, 25, 25);"
        "border: 2 solid rgb(127, 0, 127);"
        "border-radius: 7px;"
        "color: rgb(255, 255, 255);"
        "}";

    focusStyleW = "QLineEdit {"
        "background-color: rgb(25, 25, 25);"
        "border: 2 solid rgb(255, 0, 255);"
        "border-radius: 7px;"
        "color: rgb(255, 255, 255);"
        "}";

	QLocale::setDefault(QLocale(QLocale::English));
    this->setPlaceholderText("0.0");
    this->setMaxLength(50);

    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setMinimumSize(0, 0);
}

void NumberOperatorLineEdit::setCoordinate(Coordinate c)
{
    this->_coord = c;

    switch (c)
    {
    case X:
        this->setPlaceholderText("X");
        break;
    case Y:
        this->setPlaceholderText("Y");
        break;
    case Z:
        this->setPlaceholderText("Z");
        break;
    case W:
        this->setPlaceholderText("W");
        break;
    default:
        break;
    }
}

void NumberOperatorLineEdit::keyPressEvent(QKeyEvent* event)
{
    QString currentText = text();
    QChar inputChar = event->text().isEmpty() ? QChar() : event->text().at(0);

    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right || event->key() == Qt::Key_Down || event->key() == Qt::Key_Up)
    {
        QLineEdit::keyPressEvent(event);
        return;
    }

    if (inputChar.isDigit() || QString("+-*/.").contains(inputChar)) {
        QLineEdit::keyPressEvent(event); // Accetta l'input
    }
    else if (event->key() == Qt::Key_Comma || event->key() == Qt::Key_Period) {
        // Permetti la virgola o il punto come separatori decimali
        if (text().contains(QRegularExpression("[+\\-*/]$"))) {
            // Se l'ultimo carattere è un operatore, permetti di inserire un numero con la virgola
            QLineEdit::keyPressEvent(event);
        }
    }
    else if (event->key() == Qt::Key_Backspace) {
        // Gestisci il tasto Backspace per rimuovere correttamente
        QLineEdit::keyPressEvent(event);
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        // Gestisci il tasto Invio per validare l'espressione (opzionale)
        evaluateExpression();
    }
    else if (event->key() == Qt::Key_Escape) {
		// Gestisci il tasto Esc per annullare l'input
		clear();
	}
    else {
        event->ignore();
    }
}

void NumberOperatorLineEdit::paintEvent(QPaintEvent* event)
{
    if (this->hasFocus())
    {
        switch (this->_coord)
        {
        case X:
        {
            this->setStyleSheet(focusStyleX);
            break;
        }
        case Y:
        {
            this->setStyleSheet(focusStyleY);
            break;
        }
        case Z:
        {
            this->setStyleSheet(focusStyleZ);
            break;
        }
        case W:
        {
            this->setStyleSheet(focusStyleW);
            break;
        }
        default:
            break;
        }
    }
    else
    {
        switch (this->_coord)
        {
        case X:
        {
            this->setStyleSheet(normalStyleX);
            break;
        }
        case Y:
        {
            this->setStyleSheet(normalStyleY);
            break;
        }
        case Z:
        {
            this->setStyleSheet(normalStyleZ);
            break;
        }
        case W:
        {
            this->setStyleSheet(normalStyleW);
            break;
        }
        default:
            break;
        }
    }

	QLineEdit::paintEvent(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
}

void NumberOperatorLineEdit::focusInEvent(QFocusEvent* event)
{
    QLineEdit::focusInEvent(event);
}

void NumberOperatorLineEdit::focusOutEvent(QFocusEvent* event)
{
    QLineEdit::focusOutEvent(event);
}

void NumberOperatorLineEdit::evaluateExpression()
{
    QString input = text();
    input.replace(",", "."); // Supporta numeri con la virgola

    // Espressione regolare per tokenizzare numeri e operatori
    QRegularExpression regex("(?:\\d+\\.\\d+|\\.\\d+|\\d+)|[-+*/]");

    QRegularExpressionMatchIterator it = regex.globalMatch(input);

    QVector<QString> tokens;
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        tokens.append(match.captured(0)); // Usa captured(0) per prendere l'intero match
    }

    if (tokens.isEmpty()) {
        qDebug() << "Espressione vuota";
		setText(QString::number(0));
        return;
    }

    // Calcolo dell'espressione
    double result = 0.0;
    bool expectOperator = false;
    QString lastOperator = "+";

    qDebug() << "Tokens: " << tokens;

    for (const QString& token : tokens) {
        if (expectOperator) {
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                lastOperator = token;
                expectOperator = false;
            }
            else {
                qDebug() << "Operatore mancante!";
                return;
            }
        }
        else {
            if (token == "+")
                break;
            if (token == "-")
            {
                lastOperator = "-";
                continue;
            }

            bool ok = false;
            double value = token.toDouble(&ok);
            if (!ok) {
                qDebug() << "Numero non valido: " << token;
                return;
            }

            // Applica l'operatore precedente
            if (lastOperator == "+") result += value;
            else if (lastOperator == "-") result -= value;
            else if (lastOperator == "*") result *= value;
            else if (lastOperator == "/") {
                if (value == 0) {
                    qDebug() << "Errore: divisione per zero!";
                    return;
                }
                result /= value;
            }

            expectOperator = true;
        }
    }

    if (!expectOperator) {
        qDebug() << "Espressione incompleta!";
        return;
    }

    // Mostra il risultato
    setText(QString::number(result, 'g', 10));
    qDebug() << "Risultato: " << result;
}

PropertyGroupHeader::PropertyGroupHeader(const PropertyGroupHeader& other) : QFrame(other.parentWidget())
{
	this->_title = std::make_unique<QString>(*other._title);
}

PropertyGroupHeader::PropertyGroupHeader(const std::string& title, QWidget* parent) : QFrame(parent)
{
	this->_title = std::make_unique<QString>(QString(title.c_str()));
}

PropertyGroupHeader::~PropertyGroupHeader()
{
}

std::unique_ptr<QString> PropertyGroupHeader::getTitle() const { return std::make_unique<QString>(*_title); }

void PropertyGroupHeader::paintEvent(QPaintEvent* event)
{
	QFrame::paintEvent(event);

	QPainter painter(this);
	QRect rect(0, 0, width(), height());
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(255, 255, 255), 1));

	painter.drawText(rect, Qt::AlignCenter, *_title);
}

PropertyGroup::PropertyGroup(const std::string& title, QWidget* parent) : QWidget(parent), _title(title)
{
    this->_mainLayout = new QVBoxLayout(this);
    this->_header = new PropertyGroupHeader(title);
    this->_content = new PropertyGroupContainer();

    this->_mainLayout->addWidget(_header, 0, Qt::AlignTop);
    this->_mainLayout->addWidget(_content, 0, Qt::AlignTop);
    this->_mainLayout->setContentsMargins(0, 0, 0, 0);
    this->_mainLayout->setSpacing(10);
    this->_mainLayout->addStretch();

    this->_header->setStyleSheet(
        "QFrame {"
        "background-color: rgb(30, 30, 30);"
        "border-top-left-radius: 10px;"
        "border-top-right-radius: 10px;"
        "}"
    );

    this->_content->setStyleSheet(
        "QFrame {"
        "background-color: rgb(30, 30, 30);"
        "border-bottom-left-radius: 10px;"
        "border-bottom-right-radius: 10px;"
        "}"
    );

    this->_header->setFixedHeight(30);

    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->setMinimumSize(0, 0);
    this->_mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
}

const std::string& PropertyGroup::getTitle() const
{
	return this->_title;
}

QVBoxLayout* PropertyGroup::getLayout() const { return this->_content->getLayout(); }

void PropertyGroup::addElement(QWidget* elem, Qt::AlignmentFlag flag)
{
    this->_content->addElement(elem, flag);

    this->adjustSize();
}

PropertyGroupContainer::PropertyGroupContainer(QWidget* parent) : QFrame(parent)
{
    this->_mainLayout = new QVBoxLayout(this);
    this->_mainLayout->setContentsMargins(10, 10, 10, 10);
    this->_mainLayout->setSpacing(5);
    this->_mainLayout->setAlignment(Qt::AlignTop);

    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->setMinimumSize(0, 0);
    this->setMaximumHeight(0);
    this->_mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
}

QVBoxLayout* PropertyGroupContainer::getLayout() const
{
    qDebug() << "Current size:" << this->size();
    return this->_mainLayout;
}

void PropertyGroupContainer::addElement(QWidget* elem, Qt::AlignmentFlag flag)
{
    this->_mainLayout->addWidget(elem, flag);
}

void PropertyGroupContainer::paintEvent(QPaintEvent* event)
{
	QFrame::paintEvent(event);
    
    QPainterPath path;
    path.moveTo(0, 0); // Inizia dal punto superiore sinistro
    path.lineTo(width(), 0); // Linea orizzontale sopra
    path.lineTo(width(), height() - 10); // Linea verticale destra fino alla curva
    
    // Curva in basso a destra
    path.quadTo(width(), height(), width() - 10, height());
    
    path.lineTo(10, height()); // Linea orizzontale inferiore fino alla curva
    
    // Curva in basso a sinistra
    path.quadTo(0, height(), 0, height() - 10);
    
    path.lineTo(0, 0); // Linea verticale a sinistra
    path.closeSubpath(); // Chiude il percorso
    
	QPainter painter(this);
	QRect rect(0, 0, width(), height());
	painter.setRenderHint(QPainter::Antialiasing);
    painter.fillPath(path, QColor(30, 30, 30));
}