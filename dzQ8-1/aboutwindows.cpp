#include "aboutwindows.h"

AboutWindows::AboutWindows(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("About"));
    setFixedSize(200,100);
    layout = new QGridLayout();
    setLayout(layout);
    abLabel = new QLabel(this);
    abLabel->setText(tr("Text Editor by Richard"));
    layout->addWidget(abLabel,0,0,Qt::AlignHCenter);
    okButt = new QPushButton(this);
    okButt->setText(tr("Close"));
    layout->addWidget(okButt,1,0,Qt::AlignHCenter);
    connect(okButt,SIGNAL(clicked()), this, SLOT(close()));
}
