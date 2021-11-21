#include "paper.h"

#include <QGraphicsDropShadowEffect>

Paper::Paper(PaperSize size, PaperOrientation orientation) :
    pSize(size), pOrientation(orientation)
{
    updateRect();

    setBrush(QBrush(Qt::white));
//    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    //    this->setGraphicsEffect(shadow);
}

Paper::PaperSize Paper::strToPaperSize(const QString &str)
{
    if(str == "A0") {
        return A0;
    }
    if(str == "A1") {
        return A1;
    }
    if(str == "A2") {
        return A2;
    }

    if(str == "A3") {
        return A3;
    }

    return A4;
}

//Setting rectangle size
void Paper::updateRect()
{
    QSizeF tmpSize = getRectSize(pSize);
    if(pOrientation == Landscape) {
        tmpSize.transpose();
    }

    setRect(0, 0, tmpSize.width(), tmpSize.height());
}

//Returns scene size according to size name (A0, A1...)
QSizeF Paper::getRectSize(PaperSize size)
{
    switch (size) {
    case A0: {
        return QSizeF(8410, 11890);
    }
    case A1: {
        return QSizeF(5940, 8410);
    }
    case A2: {
        return QSizeF(4200, 5940);
    }
    case A3: {
        return QSizeF(2970, 4200);
    }
    case A4: {
        return QSizeF(2100, 2970);
    }
    }
}
