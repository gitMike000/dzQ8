#include "searchwindow.h"

SearchWindow::SearchWindow(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("Search Window");
    setFixedSize(600,400);
    layout = new QGridLayout(this);
    setLayout(layout);
    searchView = new QListWidget(this);
    layout->addWidget(searchView,0,0,10,6);
    goButton = new QPushButton(this);
    goButton->setText("Go");
    layout->addWidget(goButton,11,2,1,2);
    statusLabel = new QLabel(this);
    layout->addWidget(statusLabel,12,0,1,6);
    connect(goButton,SIGNAL(clicked()), this, SLOT(on_goButton_clicked()));
}

void SearchWindow::on_goButton_clicked()
{
    if (!searchView->currentItem()) return;
    QString Path=searchView->currentItem()->text();
    emit newPath(Path);
    //lineEdit->setText(Path);
    //    QModelIndex dIndex=dirmodel->setRootPath(Path);
    //    treeView->setCurrentIndex(dIndex);
    //    QModelIndex fIndex=filemodel->setRootPath(Path);
    //    tabView->setRootIndex(fIndex);


}

void SearchWindow::newsearch()
{
    searchView->clear();
    statusLabel->clear();
}

void SearchWindow::printFindFile(QString file)
{
    searchView->addItem(file);
}

void SearchWindow::changStatusLabel(QString path)
{
    statusLabel->setText(path);
}
