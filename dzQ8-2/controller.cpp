#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
}

Controller::~Controller()
{
    if (findThread != nullptr)
    // При завершении нужно остановить поток,
    // если он существует
    {
    if (findThread->isRunning()) // Проверяем, работает ли поток
    {
        findThread->findStop(); // Меняем флаг для завершения всех циклов потока
        findThread->terminate(); // Ожидаем завершения потока
    }
    findThread->quit(); // Сбрасываем поток, вызывая деструктор
    }
}

void Controller::startFind(QString dir, QString file)
{
    if (findThread != nullptr)
    {
        if (findThread->isRunning())
        {
            findThread->findStop();
            findThread->terminate();
//        findThread->exit();
//        return;
        }
    }
    findThread = new ThreadFinder(dir, file);
    findThread->start(QThread::NormalPriority);
    connect(findThread, &ThreadFinder::stopedThread, this,
    &Controller::deleteThread);
    connect(findThread, static_cast<void(ThreadFinder::*)(QString)>(&ThreadFinder::writeFoundPath), this,
    static_cast<void(Controller::*)(QString)>(&Controller::printCurrentPath));
    connect(findThread, static_cast<void(ThreadFinder::*)(QString)>(&ThreadFinder::findFile), this,
            static_cast<void(Controller::*)(QString)>(&Controller::genStringPathFile));

    emit newFind();
}

void Controller::deleteThread()
{
    findThread->findStop();
    findThread->terminate();
    //findThread->quit();
}

void Controller::printCurrentPath(QString path)
{
    currentPath = path;
    emit changFindPath(path);
}

void Controller::genStringPathFile(QString file)
{
    QString s = currentPath + file;
     // Формируем полный путь к файлу
    emit genPathOfFile(s);
    // Генерируем сигнал о готовности строки
    // файла
}

void Controller::finishThread()
{
    emit changFindPath(tr("FINISH. Find complete"));
}



