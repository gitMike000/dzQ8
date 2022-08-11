#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QApplication>
#include <QResource>
#include <QLibraryInfo>
#include <QKeyEvent>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    ui->langBox->addItems(QStringList() << "en_EN" << "ru_RU");
    connect(ui->langBox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            [=](const QString &str){
                translater.load("switchlang_" + str, ".");
                qtranslator.load("qt_" + str, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
                qApp->installTranslator(&translater);
                qApp->installTranslator(&qtranslator);

                int i = ui->themeBox->currentIndex();
                ui->themeBox->clear();
                ui->themeBox->addItems(QStringList() << tr("Light") << tr("Dark"));
                ui->themeBox->setCurrentIndex(i);
            });
    translater.load(QString("switchlang_") + QLocale::system().name());
    ui->langBox->setCurrentText(QLocale::system().name());
    qApp->installTranslator(&translater);
    ui->newEdit->setText(ui->newButton->shortcut().toString());
    ui->openEdit->setText(ui->openButton->shortcut().toString());
    ui->saveEdit->setText(ui->saveButton->shortcut().toString());
    ui->quitEdit->setText(ui->quitButton->shortcut().toString());

    ui->themeBox->setCurrentIndex(0);

    filemenu = menuBar()->addMenu(tr("File"));
    newAct = filemenu->addAction(tr("New"));
    connect(newAct, SIGNAL(triggered(bool)), this, SLOT(on_newButton_clicked()));    

    openAct = filemenu->addAction(tr("Open"));
    connect(openAct, SIGNAL(triggered(bool)), this, SLOT(on_openButton_clicked()));    

    saveAct = filemenu->addAction(tr("Save"));
    connect(saveAct, SIGNAL(triggered(bool)), this, SLOT(on_saveButton_clicked()));

    printAct = filemenu->addAction(tr("Print"));
    connect(printAct, SIGNAL(triggered(bool)), this, SLOT(on_printButton_clicked()));

    filemenu->addSeparator();
    quitAct = filemenu->addAction(tr("Quit"));
    connect(quitAct, SIGNAL(triggered(bool)), this, SLOT(on_quitButton_clicked()));

    optmenu = menuBar()->addMenu(tr("Option"));

    ui->checkBox->setChecked(true);
    readAct = optmenu->addAction(tr("ReadOnly"));
    readAct->setCheckable(true);
    readAct->setChecked(ui->checkBox->isChecked());
    //connect(readAct, SIGNAL(triggered(bool)), this, SLOT(on_checkBox_clicked(bool checked)));
    connect(readAct, &QAction::triggered, this, &MainWindow::on_checkBox_clickedM);

    langGroup = new QActionGroup(this);

    enLangAct = new QAction(tr("English"), this);
    enLangAct->setCheckable(true);
    connect(enLangAct, SIGNAL(triggered()), this, SLOT(enSlot()));

    ruLangAct = new QAction(tr("Russian"), this);
    ruLangAct->setCheckable(true);
    connect(ruLangAct, SIGNAL(triggered()), this, SLOT(ruSlot()));

    langMenu = optmenu->addMenu(tr("Language"));
    langGroup->addAction(enLangAct);
    langMenu->addAction(enLangAct);
    langGroup->addAction(ruLangAct);
    langMenu->addAction(ruLangAct);
    if (QLocale::system().name() == "ru_RU")
    {
        ruLangAct->setChecked(true);
        //enLangAct->setChecked(false);
    }
    else
    {
        enLangAct->setChecked(true);
        //ruLangAct->setChecked(false);
    }

    refmenu = menuBar()->addMenu(tr("Reference"));
    helpAct = refmenu->addAction(tr("Help"));
    connect(helpAct, SIGNAL(triggered(bool)), this, SLOT(on_helpButton_clicked()));

    aboutAct = refmenu->addAction(tr("About"));
//    connect(aboutAct, SIGNAL(triggered(bool)), this, SLOT(on_about_clicked()));
    connect(aboutAct, &QAction::triggered, this,&MainWindow::on_about_clicked);

    QToolBar *tBar = addToolBar(tr("Select"));

    formBut = new QPushButton(tr("Format"), this);
    tBar->addWidget(formBut);
    connect(formBut, &QPushButton::clicked, this, &MainWindow::formatcopy);

    QPushButton *leBut = new QPushButton(tr("Left"), this);
    tBar->addWidget(leBut);
    connect(leBut, &QPushButton::clicked, this, &MainWindow::leftAlign);

    QPushButton *riBut = new QPushButton(tr("Right"), this);
    tBar->addWidget(riBut);
    connect(riBut, &QPushButton::clicked, this, &MainWindow::rightAlign);

    QPushButton *ceBut = new QPushButton(tr("Center"), this);
    tBar->addWidget(ceBut);
    connect(ceBut, &QPushButton::clicked, this, &MainWindow::centerAlign);

    QPushButton *fontBut = new QPushButton(tr("Font"), this);
    tBar->addWidget(fontBut);
    connect(fontBut, &QPushButton::clicked, this, &MainWindow::setFont);

    QPushButton *dataBut = new QPushButton(tr("D"), this);
    tBar->addWidget(dataBut);
    connect(dataBut, &QPushButton::clicked, this, &MainWindow::setDate);

    QPushButton *timeBut = new QPushButton(tr("T"), this);
    tBar->addWidget(timeBut);
    connect(timeBut, &QPushButton::clicked, this, &MainWindow::setTime);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newButton_clicked()
{
    fileName = "";
    ui->textEdit->clear();
}

void MainWindow::on_saveButton_clicked()
{
    fileName = QFileDialog::getSaveFileName(this,
                                     tr("Save text"), Path+'/'+fileName, (tr("Text Files (*.txt)")),
                                     nullptr, QFileDialog::DontUseNativeDialog);
    if (fileName.length())
    {
        QFile f(fileName + ".txt");
        f.open(QIODevice::WriteOnly);
        QTextStream in (&f);
        in << ui->textEdit->toPlainText();
        f.flush();
        f.close();
    }
}


void MainWindow::on_openButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
                                     tr("Open text"), Path, (tr("Text Files (*.txt)")),
                                            nullptr, QFileDialog::DontUseNativeDialog );
    if (fileName.length())
    {
        Path = QFileInfo(fileName).path();
        QFile f(fileName);
        f.open(QIODevice::ReadOnly);
        QTextStream in(&f);
        ui->textEdit->setText(in.readAll());
        f.close();
    }
}


void MainWindow::on_helpButton_clicked()
{
    QFile h(":/help/text_"+ui->langBox->currentText());
    h.open(QIODevice::ReadOnly);
    QTextStream in(&h);
    ui->textEdit->setText(in.readAll());
    h.close();
}

//void MainWindow::on_checkBox_clicked(bool checked)
void MainWindow::on_checkBox_clicked()
{
    ui->textEdit->setReadOnly(ui->checkBox->isChecked());
    readAct->setChecked(ui->checkBox->isChecked());
}

void MainWindow::on_checkBox_clickedM()
{
    ui->textEdit->setReadOnly(readAct->isChecked());
    ui->checkBox->setChecked(readAct->isChecked());
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
            ui->retranslateUi(this);
            this->retranslate_menu();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
//    if (event->key() == (Qt::CTRL & Qt::Key_Q))
//    {
//        emit ui->openButton->clicked();//  on_openButton_clicked();
//    }
//    else if (event->key() == (Qt::CTRL & Qt::Key_N))
//    {
//        emit MainWindow::on_newButton_clicked();
//    }
}


void MainWindow::on_quitButton_clicked()
{
    qApp->exit(0);
}


void MainWindow::on_newEdit_textChanged(const QString &arg)
{
    ui->newButton->setShortcut(arg);
}


void MainWindow::on_openEdit_textChanged(const QString &arg)
{
    ui->openButton->setShortcut(arg);
}


void MainWindow::on_saveEdit_textChanged(const QString &arg)
{
    ui->saveButton->setShortcut(arg);
}


void MainWindow::on_quitEdit_textChanged(const QString &arg)
{
    ui->quitButton->setShortcut(arg);
}


void MainWindow::on_themeBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == tr("Light"))
    {
        setStyleSheet("QWidget {background: base; color: base;}");

    }
    else if (arg1 == tr("Dark"))
    {       
      setStyleSheet(" QWidget {color: rgb(2,203,204); background-color: rgb(68,68,68); selection-color: white; selection-background-color: gray;}");
    }
}

void MainWindow::on_about_clicked()
{
    QSharedPointer<AboutWindows> abDialog = QSharedPointer<AboutWindows>::create(this);
    abDialog->exec();
}

void MainWindow::on_printButton_clicked()
{
    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle(tr("Print"));
    if (dlg.exec() != QDialog::Accepted)
        return;

    QString printStr = ui->textEdit->toPlainText();
    QChar *list = printStr.data();
    QStringList strlst;
    int line = 0, cursor = 0;
    for (bool getst = true; getst;)
    {
        int index = printStr.indexOf("\n", cursor);
        QString s = "";
        if (index == -1)
        {
            getst = false;
            s.append(&list[cursor], printStr.length() - cursor);
        }
        else s.append(&list[cursor], index - cursor);
        cursor = index + 1;
        strlst << s;
    }
    QPainter painter;
    painter.begin(&printer);
    int w = painter.window().width();
    int h = painter.window().height();
    int amount = strlst.count();
    QFont font = painter.font();
    QFontMetrics fmetrics(font);
    for (int i = 0; i < amount; i++)
    {
        QPointF pf;
        pf.setX(10);
        pf.setY(line);
        painter.drawText(pf, strlst.at(i));
        line += fmetrics.height();
        if (h - line <= fmetrics.height())
        {
            printer.newPage();
            line = 0;
        }
    }
    painter.end();
}

void MainWindow::enSlot()
{
    ui->langBox->setCurrentIndex(0);
    emit ui->langBox->currentIndexChanged(0);
}

void MainWindow::ruSlot()
{
    ui->langBox->setCurrentIndex(1);
    emit ui->langBox->currentIndexChanged(1);
}

void MainWindow::retranslate_menu()
{
    filemenu->setTitle(tr("File"));
    newAct->setText(tr("New"));
    openAct->setText(tr("Open"));
    saveAct->setText(tr("Save"));
    printAct->setText(tr("Print"));
    quitAct->setText(tr("Quit"));

    optmenu->setTitle(tr("Option"));
    readAct->setText(tr("ReadOnly"));
    enLangAct->setText(tr("English"));
    ruLangAct->setText(tr("Russian"));

    langMenu->setTitle(tr("Language"));

    refmenu->setTitle(tr("Reference"));
    helpAct->setText(tr("Help"));
    aboutAct->setText(tr("About"));

    if (1 == ui->langBox->currentIndex())
    {
        ruLangAct->setChecked(true);
    }
    else
    {
        enLangAct->setChecked(true);
    }
}

void MainWindow::setFont()
{
    QFont font = ui->textEdit->textCursor().charFormat().font(); // получаем текущий шрифт
    QFontDialog fntDlg(font,this);
    bool b[] = {true};
    font = fntDlg.getFont(b);
    if (b[0])
    {
        QTextCharFormat fmt;
        fmt.setFont(font);
        ui->textEdit->textCursor().setCharFormat(fmt);
    }
}

void MainWindow::formatcopy()
{
    if ( formBut->text() == "COPIED" )
    {
        ui->textEdit->textCursor().setCharFormat(form);
        formBut->setText(tr("Format"));
    }
    else
    {
        form = (ui->textEdit->textCursor().charFormat().toCharFormat());
        formBut->setText(tr("COPIED"));
    }
}

void MainWindow::leftAlign()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignLeft);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::rightAlign()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::centerAlign()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(textBlockFormat);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::setDate()
{
    ui->textEdit->insertPlainText(QDateTime::currentDateTime().date().toString("dd MMMM yyyy"));
}

void MainWindow::setTime()
{
    ui->textEdit->insertPlainText(QDateTime::currentDateTime().time().toString("hh:mm"));
}
