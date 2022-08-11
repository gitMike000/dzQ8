#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QFileDialog>
#include <QSharedPointer>
#include <QActionGroup>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>

#include <QToolBar>
#include <QFontDialog>
#include <QFont>
#include <QTextCharFormat>

#include <QDateTime>

#include "aboutwindows.h"

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class MainWindow : public QMainWindow
{
    Q_OBJECT

      public:
          MainWindow(QWidget *parent = nullptr);
         ~MainWindow();

      private:
         void retranslate_menu();         

      private slots:
          void on_saveButton_clicked();
          void on_openButton_clicked();
          void on_printButton_clicked();
          void on_helpButton_clicked();
          //void on_checkBox_clicked(bool checked);
          void on_checkBox_clicked();
          void on_checkBox_clickedM();
          void changeEvent(QEvent *event);

          void keyReleaseEvent(QKeyEvent *event) override;

          void on_newButton_clicked();

          void on_quitButton_clicked();

          void on_about_clicked();

          void on_newEdit_textChanged(const QString &arg1);

          void on_openEdit_textChanged(const QString &arg1);

          void on_saveEdit_textChanged(const QString &arg1);

          void on_quitEdit_textChanged(const QString &arg1);

          void on_themeBox_currentTextChanged(const QString &arg1);

          void enSlot();

          void ruSlot();

          void setFont();

          void formatcopy();

          void leftAlign();

          void rightAlign();

          void centerAlign();

          void setDate();

          void setTime();

    private:
          Ui::MainWindow *ui;
          QString Path = "./";
          QTranslator translater, qtranslator;
          QString fileName = "";

          //QSharedPointer<AboutWindows> abDialog;
          QMenu *filemenu;
          QAction *newAct;
          QAction *openAct;
          QAction *saveAct;
          QAction *printAct;
          QAction *quitAct ;

          QMenu *optmenu;
          QAction *readAct;
          QMenu* langMenu;

          QMenu *refmenu;
          QAction *helpAct;
          QAction *aboutAct;

          QActionGroup* langGroup;
          QAction* enLangAct;
          QAction* ruLangAct;

          QPushButton *formBut;

          QTextCharFormat form;

};
#endif // MAINWINDOW_H
