#ifndef ABOUTWINDOWS_H
#define ABOUTWINDOWS_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

class AboutWindows : public QDialog
{
    Q_OBJECT
public:
    explicit AboutWindows(QWidget *parent = nullptr);

public slots:

private:
    QGridLayout* layout;
    QLabel*      abLabel;
    QPushButton* okButt;

signals:
    void on_okButt_clicked();

};

#endif // ABOUTWINDOWS_H
