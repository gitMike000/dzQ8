#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include "threadfinder.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    void startFind(QString dir, QString file);

private:
    ThreadFinder *findThread = nullptr;

private:
    QString currentPath;

signals:
    void changFindPath(QString);
    // Сообщаем о новом каталоге поиска и об
    // окончании работы
    void genPathOfFile(QString);
     // Найденный файл для вывода готов
    void newFind();
     // Очистка поля вывода результата

public slots:
    void deleteThread(); // Удаляем поток при завершение работы
    void printCurrentPath(QString); // Выводим текущий каталог поиска
    void genStringPathFile(QString); // Выводим найденный файл
    void finishThread(); // Информируем о завершении потока

    };

#endif // CONTROLLER_H
