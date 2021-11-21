#ifndef IREADERWRITER_H
#define IREADERWRITER_H

#include <QString>
#include <diagramscene.h>

class IReaderWriter
{
public:
    enum Status {
        Succes, OpenError, ReadError, WriteError
    };

    virtual Status write(QString filename, DiagramScene * const scene) = 0;
    virtual Status read(QString filename, DiagramScene * output_scene) = 0;

    virtual ~IReaderWriter() = default;
};

#endif // IREADERWRITER_H
