#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QDialog>

//#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

class SearchWindow : public QDialog
//class SearchWindow : public QWidget
{
    Q_OBJECT
public:
     explicit SearchWindow(QDialog *parent = nullptr);

public slots:
    void on_goButton_clicked();
    void newsearch();
    void printFindFile(QString file);
    void changStatusLabel(QString path);    

private:
    QListWidget *searchView;
    QPushButton *goButton;
    QGridLayout *layout;
    QLabel *statusLabel;

signals:
    void newPath(QString Path);
};

#endif // SEARCHWINDOW_H
