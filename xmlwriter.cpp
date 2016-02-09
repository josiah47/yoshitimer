#include <QtGui>

#include "xmlwriter.h"

XmlWriter::XmlWriter(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
    xml.setAutoFormatting(true);
}

bool XmlWriter::writeFile(QIODevice *device)
{
    xml.setDevice(device);

    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE xml>");
    xml.writeStartElement("xml");
    xml.writeAttribute("version", "1.0");
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        writeItem(treeWidget->topLevelItem(i));
    }
    xml.writeEndDocument();
    return true;
}

void XmlWriter::writeItem(QTreeWidgetItem *item)
{
    xml.writeStartElement("exercise");
    xml.writeTextElement("id", item->text(0));
    xml.writeTextElement("action", item->text(1));
    xml.writeTextElement("time", item->text(2));
    xml.writeTextElement("description", item->text(3));
    xml.writeEndElement();
}
