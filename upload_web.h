#ifndef UPLOAD_WEB_H
#define UPLOAD_WEB_H

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QTextEdit>
#include <QByteArray>



class Upload_web : public QMainWindow
{
    Q_OBJECT
public slots:
    void Ontop();
    void Select_cont();
    void Select_file();
    void Ok_btn_onclick();
    void firstedit_change();
    void submit_btn_onclick();

public:
    Upload_web(QWidget *parent = 0);
    ~Upload_web();
private:
    QByteArray text_ary;
    bool Isontop;
    bool Isimgshow;
    QLabel *imglab;
    QLabel *lab1;
    QLabel *lab2;
    QLineEdit *lineedit1;
    QLineEdit *lineedit2;
    QPushButton *btn;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *Ontopbtn;
    QGridLayout *mainlayout;
    QVBoxLayout *Vlayout;

    QPushButton *submit_btn;
    QLabel *firstname;
    QLabel *lastname;
    QLabel *describe;
    QLineEdit *firstedit;
    QLineEdit *lastedit;
    QTextEdit *describe_edit;
    QGridLayout *secondlayout;

    QWidget *widget;
};

#endif // UPLOAD_WEB_H
