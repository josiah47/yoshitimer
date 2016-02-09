#include "yoshitimer.h"
#include <QString>
#include <QDebug>

yoshitimer::yoshitimer(QWidget *parent)
    : QWidget(parent)
{
    //Style Sheet
    setStyleSheet("QPushButton { min-width: 300px; min-height: 75px; border-radius: 6px; border: 2px solid #8f8f91; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);}");
    // Sound
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    Phonon::createPath(mediaObject, audioOutput);
    num = new QLCDNumber(this);
    num->setNumDigits(8);
    num->setSegmentStyle(QLCDNumber::Filled);
    num->setStyleSheet("* {border-radius: 12px; border: 4px solid #ffffff; background-color:rgb(80,110,180);color:rgb(255,255,255);}");
    currentExercise = new QLCDNumber(this);
    currentExercise->setNumDigits(2);
    currentExercise->setSegmentStyle(QLCDNumber::Flat);
    currentExercise->setStyleSheet("* {border-radius: 6px; border: 2px solid #8f8f91; background-color:rgb(255,255,255);}");
    currentSet = new QLCDNumber(this);
    currentSet->setNumDigits(2);
    currentSet->setSegmentStyle(QLCDNumber::Flat);
    currentSet->setStyleSheet("* {border-radius: 6px; border: 2px solid #8f8f91; background-color:rgb(255,255,255);}");
    currentSuperSet = new QLCDNumber(this);
    currentSuperSet->setNumDigits(2);
    currentSuperSet->setSegmentStyle(QLCDNumber::Flat);
    currentSuperSet->setStyleSheet("* {border-radius: 6px; border: 2px solid #8f8f91; background-color:rgb(255,255,255);}");
    QLabel *labelCurrentExercise = new QLabel(this);
    labelCurrentExercise->setText("Current Exercise:");
    QLabel *labelCurrentSet = new QLabel(this);
    labelCurrentSet->setText("Current Set:");
    QLabel *labelCurrentSuperSet = new QLabel(this);
    labelCurrentSuperSet->setText("Current Superset:");
    timeSet = new QTime();
    timeSet->setHMS(0,0,0,0);
    //Start Delay timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    //Work timer
    workTimer = new QTimer(this);
    connect(workTimer, SIGNAL(timeout()), this, SLOT(superSets()));
    i=0;
    exerciseNumber=1;
    setNumber=1;
    superSetNumber=1;
    workFlag = false;
    delayFlag = true;
    restFlag = false;
    QLabel *labelDelay = new QLabel(this);
    labelDelay->setText("Start Delay:");
    QLabel *labelWork = new QLabel(this);
    labelWork->setText("Workout Time:");
    QLabel *labelRest = new QLabel(this);
    labelRest->setText("Set Rest Time:");
    QLabel *labelRestExercise = new QLabel(this);
    labelRestExercise->setText("Exercise Rest Time:");
    QLabel *labelExercises = new QLabel(this);
    labelExercises->setText("Exercises/Set:");
    QLabel *labelSets = new QLabel(this);
    labelSets->setText("Sets/Superset:");
    QLabel *labelSuperSets = new QLabel(this);
    labelSuperSets->setText("Number of Supersets:");
    timeEditDelay = new QTimeEdit(this);
    timeEditDelay->setDisplayFormat("hh:mm:ss");
    timeEditDelay->setStyleSheet("* { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #506eb4);}");
    timeEditWork = new QTimeEdit(this);
    timeEditWork->setDisplayFormat("hh:mm:ss");
    timeEditWork->setStyleSheet("* { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #00ff00);}");
    timeEditRest = new QTimeEdit(this);
    timeEditRest->setDisplayFormat("hh:mm:ss");
    timeEditRest->setStyleSheet("* { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #ff0000);}");
    timeEditRestExercise = new QTimeEdit(this);
    timeEditRestExercise->setDisplayFormat("hh:mm:ss");
    timeEditRestExercise->setStyleSheet("* { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #ffff00);}");
    spinBoxExercises = new QSpinBox(this);
    spinBoxExercises->setValue(1);
    spinBoxSets = new QSpinBox(this);
    spinBoxSets->setValue(1);
    spinBoxSuperSets = new QSpinBox(this);
    spinBoxSuperSets->setValue(1);
    layout = new QVBoxLayout(this);
    hlayout = new QHBoxLayout(this);
    qlayout = new QGridLayout(this);
    reset = new QPushButton("Reset",this);
    start = new QPushButton("&Start",this);
    resume = new QPushButton("&Resume",this);
    pause = new QPushButton("&Pause",this);
    pause->hide();
    resume->hide();
    QString text = timeSet->toString("hh:mm:ss");
    num->display(text);
    connect(reset,SIGNAL(clicked()),this,SLOT(resetTime()));
    connect(start,SIGNAL(clicked()),this,SLOT(startTime()));
    connect(pause,SIGNAL(clicked()),this,SLOT(pauseTime()));
    connect(resume,SIGNAL(clicked()),this,SLOT(resumeTime()));
    qlayout->addWidget(labelDelay,0,0,Qt::AlignRight);
    qlayout->addWidget(timeEditDelay,0,1);
    qlayout->addWidget(labelWork,1,0,Qt::AlignRight);
    qlayout->addWidget(timeEditWork,1,1);
    qlayout->addWidget(labelRest,2,0,Qt::AlignRight);
    qlayout->addWidget(timeEditRest,2,1);
    qlayout->addWidget(labelRestExercise,3,0,Qt::AlignRight);
    qlayout->addWidget(timeEditRestExercise,3,1);
    qlayout->addWidget(labelExercises,4,0,Qt::AlignRight);
    qlayout->addWidget(spinBoxExercises,4,1);
    qlayout->addWidget(labelSets,5,0,Qt::AlignRight);
    qlayout->addWidget(spinBoxSets,5,1);
    qlayout->addWidget(labelSuperSets,6,0,Qt::AlignRight);
    qlayout->addWidget(spinBoxSuperSets,6,1);
    qlayout->addWidget(labelCurrentExercise,0,2,2,1,Qt::AlignRight);
    qlayout->addWidget(currentExercise,0,3,2,1,Qt::AlignLeft);
    qlayout->addWidget(labelCurrentSet,2,2,2,1,Qt::AlignRight);
    qlayout->addWidget(currentSet,2,3,2,1,Qt::AlignLeft);
    qlayout->addWidget(labelCurrentSuperSet,4,2,2,1,Qt::AlignRight);
    qlayout->addWidget(currentSuperSet,4,3,2,1,Qt::AlignLeft);
    hlayout->addWidget(start);
    hlayout->addWidget(pause);
    hlayout->addWidget(resume);
    hlayout->addWidget(reset);
    qlayout->addLayout(hlayout,7,0,1,4,Qt::AlignCenter);
    layout->addWidget(num);
    layout->addLayout(qlayout);
    setLayout(layout);
    resize(800,400);
}

yoshitimer::~yoshitimer()
{
    // No need to delete any object that has a parent which is properly deleted.

}

void yoshitimer::showTime()
{
    workTimeoutTime = workTimeoutTime-1000;
    QTime newtime;
    i=i+1;
    newtime=timeSet->addSecs(-i);
    QString text = newtime.toString("hh:mm:ss");
    num->display(text);
}

void yoshitimer::countDown(int hours, int minutes, int seconds, int timeInterval)
{
    timeSet->setHMS(0,0,0);
    QString text = timeSet->toString("hh:mm:ss");
    num->display(text);
    i=0;
    timeSet->setHMS(hours, minutes, seconds);
    timer->start(1000);
    workTimer->start(timeInterval * 1000);
    workTimeoutTime = workTimer->interval();
}

void yoshitimer::resetTime()
{
    timeSet->setHMS(0,0,0);
    QString text = timeSet->toString("hh:mm:ss");
    num->display(text);
    i=0;
    pause->hide();
    resume->hide();
    start->show();
    timer->stop();
    workTimer->stop();
    exerciseNumber=1;
    setNumber=1;
    superSetNumber=1;
    workFlag = false;
    restFlag = false;
    delayFlag = true;
    currentExercise->display(0);
    currentSet->display(0);
    currentSuperSet->display(0);
    num->setStyleSheet("* {border-radius: 12px; border: 4px solid #ffffff; background-color:rgb(80,110,180);color:rgb(255,255,255);}");
}

void yoshitimer::startTime()
{
    start->hide();
    pause->show();
    setValue = spinBoxSets->value();
    exerciseValue = spinBoxExercises->value();
    superSetValue = spinBoxSuperSets->value();
    superSets();
}

void yoshitimer::pauseTime()
{
    pause->hide();
    start->hide();
    resume->show();
    timer->stop();
    workTimer->stop();
}

void yoshitimer::resumeTime()
{
    pause->show();
    start->hide();
    resume->hide();
    timer->start();
    workTimer->start(workTimeoutTime);
}

void yoshitimer::superSets()
{   
    if (superSetNumber <= superSetValue && !delayFlag) {
        currentSuperSet->display(superSetNumber);
        sets();
    } else if (superSetNumber <= superSetValue && delayFlag) {
        workFlag = true;
        delayFlag = false;
        delay();
    } else {
        resetTime();
        mediaObject->setCurrentSource(Phonon::MediaSource("Sounds/youaredoneeverything.wav"));
        mediaObject->play();
    }
}

void yoshitimer::sets()
{
    if (setNumber <= setValue && !restFlag) {
        currentSet->display(setNumber);
        rounds();
    } else if (restFlag){
        rest();
        restFlag = false;
    } else {
        delayFlag = true;
        setNumber=1;
        currentSet->display(0);
        superSetNumber++;
        superSets();
    }
}

void yoshitimer::rounds()
{
    if (exerciseNumber <= exerciseValue && !restExerciseFlag) {
        currentExercise->display(exerciseNumber);
        work();
        workFlag = false;
        restExerciseFlag = true;
    } else if (restExerciseFlag && !workFlag && exerciseNumber < exerciseValue) {
        exerciseNumber++;
        restExercise();
        restExerciseFlag = false;
        workFlag = true;
    } else {
        restExerciseFlag = false;
        restFlag = true;
        exerciseNumber=1;
        currentExercise->display(0);
        setNumber++;
        sets();
    }
}

void yoshitimer::delay()
{
    QTime delayTime = timeEditDelay->time();
    int delaySeconds = (delayTime.hour() * 3600) + (delayTime.minute() * 60) + delayTime.second();
    num->setStyleSheet("* {border-radius: 12px; border: 4px solid #ffffff; background-color:rgb(80,110,180);color:rgb(255,255,255);}");
    mediaObject->setCurrentSource(Phonon::MediaSource("Sounds/getready.wav"));
    mediaObject->play();
    countDown(timeEditDelay->time().hour(), timeEditDelay->time().minute(), timeEditDelay->time().second(), delaySeconds);
}

void yoshitimer::work()
{
    timer->stop();
    workTimer->stop();
    QTime delayTime = timeEditWork->time();
    int workSeconds = (delayTime.hour() * 3600) + (delayTime.minute() * 60) + delayTime.second();
    num->setStyleSheet("* {border-radius: 12px; border: 4px solid #000000; background-color:rgb(0,255,0);color:rgb(255,255,255);}");
    mediaObject->setCurrentSource(Phonon::MediaSource("Sounds/work.wav"));
    mediaObject->play();
    countDown(timeEditWork->time().hour(), timeEditWork->time().minute(), timeEditWork->time().second(), workSeconds);
}

void yoshitimer::rest()
{
    timer->stop();
    workTimer->stop();
    QTime delayTime = timeEditRest->time();
    int restSeconds = (delayTime.hour() * 3600) + (delayTime.minute() * 60) + delayTime.second();
    num->setStyleSheet("* {border-radius: 12px; border: 4px solid #000000; background-color:rgb(255,0,0);color:rgb(255,255,255);}");
    mediaObject->setCurrentSource(Phonon::MediaSource("Sounds/setcompleted.wav"));
    mediaObject->play();
    countDown(timeEditRest->time().hour(), timeEditRest->time().minute(), timeEditRest->time().second(), restSeconds);
}

void yoshitimer::restExercise()
{
    timer->stop();
    workTimer->stop();
    QTime delayTime = timeEditRestExercise->time();
    int restExerciseSeconds = (delayTime.hour() * 3600) + (delayTime.minute() * 60) + delayTime.second();
    num->setStyleSheet("* {border-radius: 12px; border: 4px solid #000000; background-color:rgb(255,255,0);color:rgb(255,255,255);}");
    mediaObject->setCurrentSource(Phonon::MediaSource("Sounds/pleaserest.wav"));
    mediaObject->play();
    countDown(timeEditRestExercise->time().hour(), timeEditRestExercise->time().minute(), timeEditRestExercise->time().second(), restExerciseSeconds);
}
