#ifndef MYFILEEXPLORER_H
#define MYFILEEXPLORER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QLineEdit>
#include <QTableView>
#include <QGridLayout>
#include <QPushButton>

// #include <QSharedPointer>
#include <QFileInfo>

#include "controller.h"
#include "searchwindow.h"

class myFileExplorer : public QWidget
{
    Q_OBJECT
public:
    explicit myFileExplorer(QWidget *parent = nullptr);

private:
    QFileSystemModel* dirmodel;
    QFileSystemModel* filemodel;

    QGridLayout* layout;
    QTreeView* treeView;
    QLineEdit* lineEdit;
    QTableView* tabView;

    QLineEdit* searchEdit;
    QPushButton* searchBut;

    Controller* controllerl;
//    QSharedPointer<SearchWindow> searchDialog = nullptr;
    SearchWindow* searchDialog = nullptr;

signals:

private slots:
    void setTextLineF(QModelIndex fIndex);
    void setTextLineD(QModelIndex dIndex);
    void findFileSlot();

    void setTextLine(QString Path);
    //void setTextLineD(QString dPath);

};

#endif // MYFILEEXPLORER_H
