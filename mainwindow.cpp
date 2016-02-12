#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xmlreader.h"
#include "xmlwriter.h"

#include <QSettings>
#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    readSettings();
    QSound::play(QDir::homePath().append("/yoshitimer/sounds/yoshitimer.wav"));
    ui->setupUi(this);
    setUpGUI();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

//
// GUI Stuff
//

void MainWindow::setUpGUI()
{
    //Populate Types for work queue
    workQueueTypes.insert("Get Ready", 0);
    workQueueTypes.insert("Set Rest", 1);
    workQueueTypes.insert("Superset Rest", 2);
    workQueueTypes.insert("Work", 3);

    // Populate comboBoxAction
    QString getReady;
    getReady.append(workQueueTypes.key(0));
    QColor getReadyColor;
    getReadyColor.setRgb( 0, 66, 255, 100);
    ui->comboBoxAction->addItem(getReady, getReadyColor);
    QModelIndex getReadyIdx = ui->comboBoxAction->model()->index(0, 0);
    ui->comboBoxAction->model()->setData(getReadyIdx, getReadyColor, Qt::BackgroundColorRole);

    QString setRest;
    setRest.append(workQueueTypes.key(1));
    QColor setRestColor;
    setRestColor.setRgb( 255, 255, 0, 100);
    ui->comboBoxAction->addItem(setRest, setRestColor);
    QModelIndex setRestIdx = ui->comboBoxAction->model()->index(1, 0);
    ui->comboBoxAction->model()->setData(setRestIdx, setRestColor, Qt::BackgroundColorRole);

    QString supersetRest;
    supersetRest.append(workQueueTypes.key(2));
    QColor supersetRestColor;
    supersetRestColor.setRgb( 255, 0, 0, 100);
    ui->comboBoxAction->addItem(supersetRest, supersetRestColor);
    QModelIndex supersetRestIdx = ui->comboBoxAction->model()->index(2, 0);
    ui->comboBoxAction->model()->setData(supersetRestIdx, supersetRestColor, Qt::BackgroundColorRole);

    QString workout;
    workout.append(workQueueTypes.key(3));
    QColor workoutColor;
    workoutColor.setRgb( 0, 255, 0, 100);
    ui->comboBoxAction->addItem(workout, workoutColor);
    QModelIndex workoutIdx = ui->comboBoxAction->model()->index(3, 0);
    ui->comboBoxAction->model()->setData(workoutIdx, workoutColor, Qt::BackgroundColorRole);

    // Populate itemColors for use with xml import
    itemColors.insert("0", getReadyColor);
    itemColors.insert("1", setRestColor);
    itemColors.insert("2", supersetRestColor);
    itemColors.insert("3", workoutColor);

    // Setup tree stuff
    ui->workQueue->setColumnCount(4);
    // Hide index column
    ui->workQueue->setColumnHidden(0, true);
    QStringList header;
    header << "ID" << "Action:" << "Time:" << "Description:";
    ui->workQueue->setHeaderLabels(header);
    ui->workQueue->setColumnWidth(1,170);
    ui->workQueue->setColumnWidth(2,80);
    //Drag & Drop stuff
    ui->workQueue->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->workQueue->setDragEnabled(true);
    ui->workQueue->viewport()->setAcceptDrops(true);
    ui->workQueue->setDropIndicatorShown(true);
    ui->workQueue->setDragDropMode(QAbstractItemView::InternalMove);

    // Context Menu For work queue
    actionDuplicate = new QAction("Duplicate", ui->workQueue);
    connect(actionDuplicate, SIGNAL(triggered()), this, SLOT(duplicateRow()));
    ui->workQueue->addAction(actionDuplicate);

    actionDelete = new QAction("Delete", ui->workQueue);
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteRow()));
    ui->workQueue->addAction(actionDelete);

    actionUp = new QAction("Move Up", ui->workQueue);
    connect(actionUp, SIGNAL(triggered()), this, SLOT(moveRowUp()));
    ui->workQueue->addAction(actionUp);

    actionDown = new QAction("Move Down", ui->workQueue);
    connect(actionDown, SIGNAL(triggered()), this, SLOT(moveRowDown()));
    ui->workQueue->addAction(actionDown);

    timeSet = new QTime();
    timeSet->setHMS(0,0,0);
    QString text = timeSet->toString("mm:ss");
    ui->lcdTime->display(text);
    QString totalText = timeSet->toString("hh:mm:ss");
    ui->lcdNumber_timeCompleted->display(totalText);
    ui->lcdNumber_timeRemaining->display(totalText);
    ui->lcdNumber_totalTime->display(totalText);

    //Set up timers
    secTimer = new QTimer(this);
    connect(secTimer, SIGNAL(timeout()), this, SLOT(showTime()));
    //Work Queue timer
    workTimer = new QTimer(this);
    connect(workTimer, SIGNAL(timeout()), this, SLOT(workQueue()));
    //Disable buttons
    ui->pushButtonStart->setDisabled(true);
    ui->pushButtonStop->setDisabled(true);
    ui->pushButtonResume->setDisabled(true);
    ui->pushButtonPause->setDisabled(true);

}

void MainWindow::playSound(int index)
{
    switch ( index ) {
        case 0:
             QSound::play(QDir::homePath().append("/yoshitimer/sounds/getready.wav"));
            break;
        case 1:
             QSound::play(QDir::homePath().append("/yoshitimer/sounds/setcompleted.wav"));
            break;
        case 2:
             QSound::play(QDir::homePath().append("/yoshitimer/sounds/supersetcompleted.wav"));
            break;
        case 3:
             QSound::play(QDir::homePath().append("/yoshitimer/sounds/work.wav"));
            break;
       case 4:
             QSound::play(QDir::homePath().append("/yoshitimer/sounds/youaredoneeverything.wav"));
            break;
        case 5:
             QSound::play(QDir::homePath().append("/yoshitimer/sounds/buzzer.wav"));
            break;
       default:
             QSound::play(QDir::homePath().append("/yoshitimer/sounds/youaredoneeverything.wav"));
            break;
    }
}

void MainWindow::duplicateRow()
{
    QModelIndex index = ui->workQueue->currentIndex();
    int rownum = index.row();
    QTreeWidgetItem *dupItem = ui->workQueue->topLevelItem(rownum);
    QTreeWidgetItem *newItem = dupItem->clone();
    ui->workQueue->addTopLevelItem(newItem);
}

void MainWindow::deleteRow()
{
    QModelIndex index = ui->workQueue->currentIndex();
    int rownum = index.row();
    ui->workQueue->takeTopLevelItem(rownum);
    if ( ui->workQueue->topLevelItemCount() == 0 ) {
        ui->pushButtonStart->setDisabled(true);
        ui->pushButtonPause->setDisabled(true);
        ui->pushButtonResume->setDisabled(true);
        ui->pushButtonStop->setDisabled(true);
    }
}

void MainWindow::moveRowUp()
{
    QModelIndex index = ui->workQueue->currentIndex();
    int rownum = index.row();
    if ( rownum == 0 ) {
        return;
    }
    QTreeWidgetItem *first = ui->workQueue->takeTopLevelItem(rownum);
    QTreeWidgetItem *second = ui->workQueue->takeTopLevelItem(rownum-1);
    ui->workQueue->insertTopLevelItem(rownum-1, first);
    ui->workQueue->insertTopLevelItem(rownum, second);
}

void MainWindow::moveRowDown()
{
    QModelIndex index = ui->workQueue->currentIndex();
    int rownum = index.row();
    if ( rownum == ui->workQueue->topLevelItemCount() - 1 ) {
        return;
    }
    QTreeWidgetItem *second = ui->workQueue->takeTopLevelItem(rownum+1);
    QTreeWidgetItem *first = ui->workQueue->takeTopLevelItem(rownum);
    ui->workQueue->insertTopLevelItem(rownum, second);
    ui->workQueue->insertTopLevelItem(rownum+1, first);

}

void MainWindow::on_pushButton_clicked()
{
    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->workQueue);
    QString comboText = ui->comboBoxAction->currentText();
    QVariant comboTextNum = workQueueTypes.value(comboText);
    QString comboTextNumString= comboTextNum.toString();
    newItem->setText(0, comboTextNumString);
    newItem->setText(1, comboText);
    newItem->setText(2, ui->timeEdit->time().toString("mm:ss"));
    newItem->setText(3, ui->lineEditDescription->text());
    QModelIndex getIdx = ui->comboBoxAction->model()->index(ui->comboBoxAction->currentIndex(), 0);
    QVariant variant = ui->comboBoxAction->model()->data(getIdx, Qt::BackgroundColorRole);
    QColor color = variant.value<QColor>();
    newItem->setBackgroundColor(1,color);
    newItem->setBackgroundColor(2,color);
    newItem->setBackgroundColor(3,color);
    newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    if ( ui->workQueue->topLevelItemCount() > 0 ) {
        ui->pushButtonStart->setDisabled(false);
    }
}

void MainWindow::on_pushButtonStart_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButtonStart->setDisabled(true);
    ui->pushButtonStop->setDisabled(false);
    ui->pushButtonPause->setDisabled(false);
    ui->pushButtonPause->setFocus();
    QModelIndex index = ui->workQueue->currentIndex();
    firstQueuedItem = index.row();
    if (firstQueuedItem == -1){
        firstQueuedItem = 0;
    }
    workQueue();
}

void MainWindow::on_pushButtonStop_clicked()
{
    secTimer->stop();
    workTimer->stop();
    ui->pushButtonStart->setDisabled(false);
    ui->pushButtonStop->setDisabled(true);
    ui->pushButtonPause->setDisabled(true);
    ui->pushButtonResume->setDisabled(true);
    ui->pushButton->setEnabled(true);
    ui->workQueue->clearSelection();
    ui->lcdTime->setStyleSheet("* {background-color:rgb(255,255,255);}");
    timeSet->setHMS(0,0,0);
    QString text = timeSet->toString("mm:ss");
    ui->lcdTime->display(text);
}

void MainWindow::on_pushButtonPause_clicked()
{
    ui->pushButtonResume->setDisabled(false);
    ui->pushButtonPause->setDisabled(true);
    ui->pushButtonStart->setFocus();
    ui->pushButton->setEnabled(true);
    secTimer->stop();
    workTimer->stop();
}

void MainWindow::on_pushButtonResume_clicked()
{
    ui->pushButtonPause->setDisabled(false);
    secTimer->start();
    workTimer->start(workTimeoutTime);
}

void MainWindow::on_actionNew_triggered()
{
    secTime=0;
    secTimer->stop();
    workTimer->stop();
    ui->pushButtonStart->setDisabled(true);
    ui->pushButtonPause->setDisabled(true);
    ui->pushButtonResume->setDisabled(true);
    ui->pushButtonStop->setDisabled(true);
    ui->pushButton->setEnabled(true);
    timeSet->setHMS(0,0,0);
    QString text = timeSet->toString("mm:ss");
    ui->lcdTime->display(text);
    ui->workQueue->clear();
}


void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Exercise File"),QDir::currentPath(),tr("XML Files (*.xml)"));
    if (fileName.isEmpty()) {
          return;
      }
      QFile file(fileName);
      if (!file.open(QFile::WriteOnly | QFile::Text)) {
          QMessageBox::warning(this, tr("Yoshi"),tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
          return;
      }
      XmlWriter writer(ui->workQueue);
      if (writer.writeFile(&file)) {
          statusBar()->showMessage(tr("File saved"), 2000);
      }
}

void MainWindow::on_actionOpen_triggered()
{
    QSettings settings;
    QByteArray lastFolderLocation = settings.value("MainWindow/openFileFolder").toByteArray();
    QFileDialog fileDialog;
    fileDialog.restoreState(lastFolderLocation);
    QString fileName = fileDialog.getOpenFileName(this, tr("Open Exercise File"),"",tr("XML Files (*.xml)"));
    if (fileName.isEmpty()) {
        return;
    }

    settings.setValue("MainWindow/openFileFolder", fileDialog.saveState());
    ui->workQueue->clear();

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Yoshi"),tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    XmlReader reader(ui->workQueue,itemColors);
    if (!reader.read(&file)) {
        QMessageBox::warning(this, tr("Yoshi"),tr("Parse error in file %1:\n\n%2").arg(fileName).arg(reader.errorString()));
    } else {
        if ( ui->workQueue->topLevelItemCount() > 0 ) {
            ui->pushButtonStart->setDisabled(false);
        }
        statusBar()->showMessage(tr("File loaded"), 2000);
    }
}

//
// Timer Stuff
//

void MainWindow::showTime()
{
    QTime newtime;
    secTime=secTime+1;
    newtime=timeSet->addSecs(-secTime);
    QString timeText = newtime.toString("mm:ss");
    ui->lcdTime->display(timeText);
    workTimeoutTime = workTimeoutTime - 1000;
}

void MainWindow::countDown(int minutes, int seconds)
{
    timeSet->setHMS(0,0,0);
    QString text = timeSet->toString("mm:ss");
    ui->lcdTime->display(text);
    secTime=0;
    timeSet->setHMS(0, minutes, seconds);
    secTimer->start(1000);
    workTimer->start(((minutes * 60) + seconds) * 1000);
    workTimeoutTime = ((minutes * 60) + seconds) * 1000;
}

void MainWindow::workQueue()
{
    // If item is last item call stop
    if ( firstQueuedItem == ui->workQueue->topLevelItemCount()) {
        on_pushButtonStop_clicked();
        playSound(4);
    } else {
        secTimer->stop();
        workTimer->stop();

        // Select current item
        QTreeWidgetItem *currentQueuedItem = ui->workQueue->topLevelItem(firstQueuedItem);

        // Move Selection down to current item
        ui->workQueue->clearSelection();
        currentQueuedItem->setSelected(true);
        ui->workQueue->scrollToBottom();
        ui->workQueue->scrollToItem(currentQueuedItem);

        // Set Background Color based on tree item background
        QColor bgcurrent = currentQueuedItem->backgroundColor(1);
        QString style = QString("QLCDNumber { background-color: %1 }").arg(bgcurrent.name());
        ui->lcdTime->setStyleSheet(style);

        // Get reference from tree for what sound file to call
        QString queuedItemWhatString = currentQueuedItem->text(0);
        bool ok;
        int queuedToSound = queuedItemWhatString.toInt(&ok, 10);
        playSound(queuedToSound);

        // Get text from current item row/column
        QString queuedItemTimeString = currentQueuedItem->text(2);

        QString setItemText;
        if (currentQueuedItem->text(3).isEmpty()) {
            setItemText =currentQueuedItem->text(1);
        } else {
            setItemText =currentQueuedItem->text(3);
        }
        ui->lineEdit_exercise->setText(setItemText);

        // Convert text from current item to time
        QTime queuedItemTime = QTime::fromString(queuedItemTimeString, "mm:ss");


        // Call countDown and send workout timer interval
        countDown(queuedItemTime.minute(), queuedItemTime.second());

        // Increment to go to next work queue item in tree
        firstQueuedItem++;
    }
}

void MainWindow::on_workQueue_itemSelectionChanged()
{
    calculateTimes();
}

void MainWindow::calculateTimes()
{
    QTime setTime(0,0,0);
    QTime remainingAdd(0,0,0);
    QTime totalTime;
    QTime timeCompleted;
    QTime timeRemaining;

    for (int i = 0; i < ui->workQueue->topLevelItemCount(); ++i) {
        QTreeWidgetItem *currentItem = ui->workQueue->topLevelItem(i);
        QTime itemTime = QTime::fromString(QString(currentItem->text(2)), "mm:ss");

        int minute = itemTime.minute();
        int second = itemTime.second();
        if (i == firstQueuedItem) {
            timeCompleted = totalTime;
        }
        if (i >= firstQueuedItem) {
            timeRemaining = remainingAdd.addSecs((timeRemaining.hour()*3600) + (timeRemaining.minute()*60) +timeRemaining.second() + (minute*60) + second);
        }

        int totalSeconds = (totalTime.hour()*3600) + (totalTime.minute()*60) +totalTime.second() + (minute*60) + second;
        totalTime = setTime.addSecs(totalSeconds);
    }
    qDebug() << timeRemaining.hour() << timeRemaining.minute() << timeRemaining.second();
    QString totalTimeText = totalTime.toString("hh:mm:ss");
    ui->lcdNumber_totalTime->display(totalTimeText);

    QString timeCompletedText = timeCompleted.toString("hh:mm:ss");
    ui->lcdNumber_timeCompleted->display(timeCompletedText);

    QString timeRemainingText = timeRemaining.toString("hh:mm:ss");
    ui->lcdNumber_timeRemaining->display(timeRemainingText);
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}
