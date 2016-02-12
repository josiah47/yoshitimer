#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QMap>
#include <QSound>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMap<QString,   int> workQueueTypes;
    QMap<QString,   QColor> itemColors;
    Ui::MainWindow  *ui;
    void            readSettings();
    void            writeSettings();
    void            setUpGUI();

    void            playSound(int index);

    QAction         *actionDuplicate;
    QAction         *actionDelete;
    QAction         *actionUp;
    QAction         *actionDown;

    QTime           *timeSet;
    QTimer          *secTimer;
    QTimer          *workTimer;
    int             secTime;
    int             workTimeoutTime;
    int             firstQueuedItem;

private slots:
    // GUI Stuff
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_As_triggered();
    void on_pushButtonStop_clicked();
    void on_pushButtonResume_clicked();
    void on_pushButtonStart_clicked();
    void on_pushButtonPause_clicked();
    void duplicateRow();
    void deleteRow();
    void moveRowUp();
    void moveRowDown();
    void on_pushButton_clicked();

    // Timer Stuff
    void showTime();
    void workQueue();
    void countDown(int minutes, int seconds);
    void on_workQueue_itemSelectionChanged();
    void calculateTimes();
};

#endif // MAINWINDOW_H
