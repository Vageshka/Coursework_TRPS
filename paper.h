#ifndef PAPER_H
#define PAPER_H

#include <QGraphicsRectItem>
#include <QPrinter>

class Paper : public QGraphicsRectItem
{
public:
    enum PaperSize {
        A0 = QPageSize::A0,
        A1 = QPageSize::A1,
        A2 = QPageSize::A2,
        A3 = QPageSize::A3,
        A4 = QPageSize::A4,
    };

    enum PaperOrientation {
        Portrait = QPrinter::Portrait,
        Landscape = QPrinter::Landscape,
    };

    Paper(PaperSize size = A4, PaperOrientation orientation = Portrait);

    void setSize(PaperSize size) { pSize = size; updateRect(); }
    void setOrientation(PaperOrientation orientation) { pOrientation = orientation; updateRect(); }
    PaperSize size() const { return pSize; }
    PaperOrientation orientation() const { return pOrientation; }

protected:
    void updateRect();
    QSizeF getRectSize(PaperSize size);

private:
    PaperSize pSize;
    PaperOrientation pOrientation;
};

#endif // PAPER_H
