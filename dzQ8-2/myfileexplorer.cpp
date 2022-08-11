//#include <QMainWindow>
//#include "mainwindow.h"
#include "myfileexplorer.h"
#include <QModelIndexList>

myFileExplorer::myFileExplorer(QWidget *parent) : QWidget(parent)
{
    dirmodel = new QFileSystemModel();
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel->setRootPath(QDir::rootPath());

    layout = new QGridLayout(this);

    treeView = new QTreeView(this);
    treeView->setModel(dirmodel);

    filemodel = new QFileSystemModel();
    filemodel->setFilter(QDir::NoDotAndDotDot|QDir::AllEntries);
    filemodel->setRootPath(QDir::rootPath());

    tabView = new QTableView(this);
    tabView->setModel(filemodel);

    lineEdit = new QLineEdit(this);
    lineEdit->setReadOnly(true);
    lineEdit->setText(QDir::rootPath());

    searchEdit = new QLineEdit(this);
    searchBut = new QPushButton(this);
    searchBut->setText("Find");

//    connect(treeView,SIGNAL(clicked(QModelIndex)),
//            tabView,SLOT(setRootIndex(QModelIndex)));
//    connect(tabView,SIGNAL(activated(QModelIndex)),
//            treeView,SLOT(setCurrentIndex(QModelIndex)));
    connect(tabView,SIGNAL(activated(QModelIndex)),
            tabView,SLOT(setRootIndex(QModelIndex)));

    connect(treeView,SIGNAL(clicked(QModelIndex)),
            this, SLOT(setTextLineD(QModelIndex)));

    connect(tabView,SIGNAL(activated(QModelIndex)),
            this,SLOT(setTextLineF(QModelIndex)));

    layout->addWidget(lineEdit,0,0,1,8);
    layout->addWidget(treeView,1,0,1,3);
    layout->addWidget(tabView,1,3,1,5);
    layout->addWidget(searchEdit,2,0,1,7);
    layout->addWidget(searchBut,2,7,1,1);

    controllerl = new Controller(this);
    searchDialog = new SearchWindow();

    connect(controllerl, SIGNAL(changFindPath(QString)), searchDialog,
            SLOT(changStatusLabel(QString)));
    connect(controllerl, SIGNAL(genPathOfFile(QString)), searchDialog,
            SLOT(printFindFile(QString)));
    connect(controllerl, SIGNAL(newFind()), searchDialog, SLOT(newsearch()));
    connect(searchBut, SIGNAL(clicked()), this, SLOT(findFileSlot()));
    connect(searchDialog,&QDialog::rejected, controllerl, &Controller::deleteThread);

    connect(parent,SIGNAL(MainWindowIsClosing()), controllerl, SLOT(deleteThread()));

    connect(searchDialog,SIGNAL(newPath(QString)), this, SLOT(setTextLine(QString)));

    this->setLayout(layout);

}

void myFileExplorer::setTextLineF(QModelIndex fIndex)
{
    QString fPath=filemodel->filePath(fIndex);
    QModelIndex dIndex=dirmodel->setRootPath(fPath);

    lineEdit->setText(fPath);
    treeView->setCurrentIndex(dIndex);
}

void myFileExplorer::setTextLineD(QModelIndex dIndex)
{
    QString dPath=dirmodel->filePath(dIndex);
    QModelIndex fIndex=filemodel->setRootPath(dPath);

    lineEdit->setText(dPath);
    tabView->setRootIndex(fIndex);
}

void myFileExplorer::setTextLine(QString Path)
{
    QFileInfo path(Path);
    QString fPath;
    QString dPath;
    if (path.isFile())
    {
        fPath = path.absolutePath();
        dPath = path.absolutePath();
    }
    else
    {
        fPath = path.absoluteFilePath();
        dPath = fPath;
    }

    QModelIndex dIndex=dirmodel->setRootPath(dPath);
    lineEdit->setText(path.absoluteFilePath());
    treeView->setCurrentIndex(dIndex);

    QModelIndex fIndex=filemodel->setRootPath(fPath);
    tabView->setRootIndex(fIndex);

}

void myFileExplorer::findFileSlot()
{
    QString linesearch = searchEdit->text();
    if (linesearch.length() == 0) return;

    searchDialog->show();
    QString dir = QFileInfo(lineEdit->text()).absoluteFilePath();
    if (dir !="/") dir+="/";
    controllerl->startFind( dir, linesearch);
}


