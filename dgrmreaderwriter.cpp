#include "dgrmreaderwriter.h"

#include <QFile>

IReaderWriter::Status DGRMReaderWriter::write(QString filename, DiagramScene * const scene)
{
    Status s = Succes;

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly)) {
        return OpenError;
    }

    try {
        QDataStream out(&file);

        //Writing paper parametrs
        out << scene->paper->size() << scene->paper->orientation()
            << scene->paper->brush().color();

        //Writing diagramItems
        const auto& dItems = scene->diagramItems();
        out << dItems.size();
        for(auto item : dItems) {
            out << item->itemType() << item->itemSubtype()
                << item->pos() << item->Size() << item->Text()
                << item->textColor() << item->font() << item->Brush().color()
                << item->Pen().widthF() << item->Pen().color();
        }

        //Writing arrows
        const auto& arrows = scene->arrows();
        out << arrows.size();
        for(auto arrow : arrows) {
            out << arrow->line() << (arrow->startItem() ? dItems.indexOf(arrow->startItem()) : -1)
                << (arrow->endItem() ? dItems.indexOf(arrow->endItem()) : -1)
                << arrow->Pen().widthF() << arrow->Pen().color()
                << arrow->arrowhead() << arrow->arrowheadLength();
        }
    }  catch (...) {
        s = WriteError;
    }

    file.close();
    return s;
}

IReaderWriter::Status DGRMReaderWriter::read(QString filename, DiagramScene *output_scene)
{
    Status s = Succes;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        return OpenError;
    }

    try {
        QDataStream in(&file);

        //Reading paper parametrs
        PaperParams paper_params;
        in >> paper_params;
        output_scene->paper->setSize(paper_params.size);
        output_scene->paper->setOrientation(paper_params.orientation);
        output_scene->paper->setBrush(paper_params.color);

        //Reading diagramItems
        QList<DiagramItem *> dItems;
        int n;
        in >> n;
        for(auto i = 0; i < n; i++) {
            ItemPaprams item_params;
            DiagramItem* item;

            in >> item_params;
            item = new RegularVertex(static_cast<RegularVertex::VertexType>(item_params.subtype));
            item->setPos(item_params.pos);
            item->setSize(item_params.size);
            item->setText(item_params.text);
            item->setTextColor(item_params.text_color);
            item->setFont(item_params.font);
            item->setBrush(item_params.color);
            item->setPen(QPen(item_params.pen_color, item_params.pen_width));

            output_scene->addItem(item);
            dItems.push_back(item);
        }

        //Reading arrows
        in >> n;
        for(auto i = 0; i < n; i++) {
            ArrowPaprams arrow_params;
            Arrow* arrow;
            in >> arrow_params;
            arrow = new Arrow(arrow_params.line);
            if(arrow_params.start_item_index != -1) {
                arrow->setStartItem(dItems[arrow_params.start_item_index]);
            }
            if(arrow_params.end_item_index != -1) {
                arrow->setEndItem(dItems[arrow_params.end_item_index]);
            }
            arrow->setPen(QPen(arrow_params.pen_color, arrow_params.pen_width));
            arrow->setArrowhead(arrow_params.head_type);
            arrow->setArrowheadLength(arrow_params.head_length);
            output_scene->addItem(arrow);
        }
    }  catch (...) {
        s = ReadError;
    }

    file.close();
    return s;
}

QDataStream &operator>>(QDataStream &stream, DGRMReaderWriter::PaperParams &params)
{
    stream >> params.size >> params.orientation >> params.color;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const DGRMReaderWriter::PaperParams &params)
{
    stream << params.size << params.orientation << params.color;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, DGRMReaderWriter::ItemPaprams &params)
{
    stream >> params.type >> params.subtype >> params.pos
           >> params.size >> params.text >> params.text_color
           >> params.font >> params.color >> params.pen_width
           >> params.pen_color;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const DGRMReaderWriter::ItemPaprams &params)
{
    stream << params.type << params.subtype << params.pos
           << params.size << params.text << params.text_color
           << params.font << params.color << params.pen_width
           << params.pen_color;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, DGRMReaderWriter::ArrowPaprams &params)
{
    stream >> params.line >> params.start_item_index
           >> params.end_item_index >> params.pen_width
           >> params.pen_color >> params.head_type
           >> params.head_length;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const DGRMReaderWriter::ArrowPaprams &params)
{
    stream << params.line << params.start_item_index
           << params.end_item_index << params.pen_width
           << params.pen_color << params.head_type
           << params.head_length;
    return stream;
}
