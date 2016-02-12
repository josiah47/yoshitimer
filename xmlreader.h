#ifndef XMLREADER_H
#define XMLREADER_H

#include <QIcon>
#include <QXmlStreamReader>
#include <QTreeWidget>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class XmlReader
{
public:
    XmlReader(QTreeWidget *treeWidget,QMap<QString, QColor> itemColors);

    bool read(QIODevice *device);

    QString errorString() const;

private:
    QMap<QString, QColor> itemColorsXML;

    void readXML();
    void readExercise();

    QXmlStreamReader xml;
    QTreeWidget *treeWidget;
    QTreeWidgetItem * createChildItem();
};

#endif
