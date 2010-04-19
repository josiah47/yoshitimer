#ifndef YOSHITIMER_H
#define YOSHITIMER_H

#include <QtGui/QWidget>
#include <QSound>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLCDNumber>
#include <QTime>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimeEdit>
#include <QSpinBox>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>

class yoshitimer : public QWidget
{
    Q_OBJECT

public:
    int i;
    int exerciseNumber;
    int exerciseValue;
    int setNumber;
    int setValue;
    int superSetNumber;
    int superSetValue;
    bool workFlag;
    bool restFlag;
    bool restExerciseFlag;
    bool delayFlag;
    yoshitimer(QWidget *parent = 0);
    ~yoshitimer();
private slots:
    void resetTime();
    void pauseTime();
    void resumeTime();
    void startTime();
    void showTime();
    void countDown(int hours, int minutes, int seconds, int timeInterval);
    void delay();
    void work();
    void rest();
    void restExercise();
    void rounds();
    void sets();
    void superSets();

private:
    int workTimeoutTime;
    QSound *bell;
    QTime *timeSet;
    QTime *delayTime;
    QTimer *timer;
    QTimer *workTimer;
    QLabel *labelCurrentExercise;
    QLabel *labelCurrentSet;
    QLabel *labelCurrentSuperSet;
    QLabel *labelDelay;
    QLabel *labelWork;
    QLabel *labelRest;
    QLabel *labelRestExercise;
    QLabel *labelExercises;
    QLabel *labelSets;
    QLabel *labelSuperSets;
    QVBoxLayout *layout;
    QHBoxLayout *hlayout;
    QGridLayout *qlayout;
    QPushButton *reset;
    QPushButton *start;
    QPushButton *pause;
    QPushButton *resume;
    QLCDNumber *num;
    QLCDNumber *currentExercise;
    QLCDNumber *currentSet;
    QLCDNumber *currentSuperSet;
    QTimeEdit *timeEditDelay;
    QTimeEdit *timeEditWork;
    QTimeEdit *timeEditRest;
    QTimeEdit *timeEditRestExercise;
    QSpinBox *spinBoxExercises;
    QSpinBox *spinBoxSets;
    QSpinBox *spinBoxSuperSets;
    //Audio Output
    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;
};

#endif // YOSHITIMER_H
