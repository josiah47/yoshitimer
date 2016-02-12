#include <QtGui>

#include "xmlreader.h"

XmlReader::XmlReader(QTreeWidget *treeWidget,QMap<QString, QColor> itemColors)
    : treeWidget(treeWidget)
{
    itemColorsXML = itemColors;
}

bool XmlReader::read(QIODevice *device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) {
        if (xml.name() == "xml" && xml.attributes().value("version") == "1.0")
            readXML();
        else
            xml.raiseError(QObject::tr("The file is not an XML version 1.0 file."));
    }

    return !xml.error();
}

QString XmlReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}

void XmlReader::readXML()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "xml");

    while (xml.readNextStartElement()) {
        if (xml.name() == "exercise")
            readExercise();
        else
            xml.skipCurrentElement();
    }
}

void XmlReader::readExercise()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "exercise");

    QTreeWidgetItem *exercise = createChildItem();

    while (xml.readNextStartElement()) {
        if (xml.name() == "id") {
            Q_ASSERT(xml.isStartElement() && xml.name() == "id");
            QString id = xml.readElementText();
            exercise->setText(0,id);
            QColor bgcolor = itemColorsXML.value(id);
            exercise->setBackgroundColor(1,bgcolor);
            exercise->setBackgroundColor(2,bgcolor);
            exercise->setBackgroundColor(3,bgcolor);
        } else if (xml.name() == "action") {
            Q_ASSERT(xml.isStartElement() && xml.name() == "action");
            QString action = xml.readElementText();
            exercise->setText(1,action);
        } else if (xml.name() == "time") {
            Q_ASSERT(xml.isStartElement() && xml.name() == "time");
            QString time = xml.readElementText();
            exercise->setText(2,time);
        } else if (xml.name() == "description") {
            Q_ASSERT(xml.isStartElement() && xml.name() == "description");
            QString description = xml.readElementText();
            exercise->setText(3,description);
        } else {
            xml.skipCurrentElement();
        }
    }
}

QTreeWidgetItem* XmlReader::createChildItem()
{
    QTreeWidgetItem * childItem = new QTreeWidgetItem(treeWidget);
    childItem->setData(0, Qt::UserRole, xml.name().toString());
    childItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    return childItem;
}
