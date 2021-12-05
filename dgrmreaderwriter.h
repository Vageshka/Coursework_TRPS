#ifndef DGRMREADERWRITER_H
#define DGRMREADERWRITER_H

#include <ireaderwriter.h>

class DGRMReaderWriter : public IReaderWriter
{
public:
    DGRMReaderWriter() {};

    struct PaperParams {
        Paper::PaperSize size;
        Paper::PaperOrientation orientation;
        QColor color;

        friend QDataStream &operator>>(QDataStream &stream, PaperParams &params);
        friend QDataStream &operator<<(QDataStream &stream, const PaperParams &params);
    };

    struct ItemPaprams {
        DiagramItem::ItemType type;
        int subtype;
        QPointF pos;
        QSizeF size;
        QString text;
        QColor text_color;
        QFont font;
        QColor color;
        double pen_width;
        QColor pen_color;

        friend QDataStream &operator>>(QDataStream &stream, ItemPaprams &params);
        friend QDataStream &operator<<(QDataStream &stream, const ItemPaprams &params);
    };

    struct ArrowPaprams {
        Arrow::ArrowType type;
        QLineF line;
        int start_item_index;
        int end_item_index;
        double pen_width;
        QColor pen_color;
        Arrow::ArrowheadType head_type;
        int head_length;

        friend QDataStream &operator>>(QDataStream &stream, ArrowPaprams &params);
        friend QDataStream &operator<<(QDataStream &stream, const ArrowPaprams &params);
    };

    virtual Status write(QString filename, DiagramScene * const scene) override;
    virtual Status read(QString filename, DiagramScene * output_scene) override;
};

#endif // DGRMREADERWRITER_H
