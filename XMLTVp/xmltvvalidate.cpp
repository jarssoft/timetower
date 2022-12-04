#include "xmltvvalidate.h"
#include <QFile>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

namespace xmltvp {

bool xmltvvalidate(const QByteArray instanceData)
{
    QFile schemafile(":/new/prefix1/xmltv.xsd");
    schemafile.open(QIODevice::ReadOnly);

    QXmlSchema schema;
    schema.load(&schemafile);
    assert(schema.isValid());

    QXmlSchemaValidator validator(schema);
    return validator.validate(instanceData);
}

}
