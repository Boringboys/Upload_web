#include "upload_web.h"
#include <QGridLayout>
#include <QLineEdit>
#include <windows.h>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <QByteArray>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDateTime>


Upload_web::Upload_web(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("增加文件");

    Isontop=false;
    Isimgshow=true;
    imglab=new QLabel;
    lab1=new QLabel("文章目录：");
    lab2=new QLabel("添加文件：");
    lineedit1=new QLineEdit;
    lineedit2=new QLineEdit;
    btn=new QPushButton("选择目录");
    btn1=new QPushButton("选择文件");
    btn2=new QPushButton("添加");
    Ontopbtn=new QPushButton("窗口置顶");

    firstname=new QLabel("一级标题：");
    lastname=new QLabel("二级标题：");
    describe=new QLabel("描述：");
    firstedit=new QLineEdit();
    lastedit=new QLineEdit();
    describe_edit=new QTextEdit();
    submit_btn=new QPushButton("确认添加");

    lastedit->setEnabled(false);
    firstname->hide();
    lastname->hide();
    describe->hide();
    firstedit->hide();
    lastedit->hide();
    describe_edit->hide();
    submit_btn->hide();

    QPixmap pixmap("img/123456.jpg");
    imglab->setPixmap(pixmap);
    imglab->resize(pixmap.width(),pixmap.height());

    secondlayout=new QGridLayout();
    secondlayout->addWidget(firstname,0,0);
    secondlayout->addWidget(firstedit,0,1,1,7);
    secondlayout->addWidget(lastname,1,0);
    secondlayout->addWidget(lastedit,1,1,1,7);
    secondlayout->addWidget(describe,2,0);
    secondlayout->addWidget(describe_edit,2,1,1,7);
    secondlayout->addWidget(submit_btn,3,7);
    secondlayout->setMargin(15);
    secondlayout->setSpacing(10);

    mainlayout=new QGridLayout();
    mainlayout->addWidget(lab1,0,0);
    mainlayout->addWidget(lineedit1,0,1,1,6);
    mainlayout->addWidget(btn,0,7);
    mainlayout->addWidget(lab2,1,0);
    mainlayout->addWidget(lineedit2,1,1,1,6);
    mainlayout->addWidget(btn1,1,7);
    mainlayout->addWidget(btn2,2,7);
    mainlayout->addWidget(Ontopbtn,2,0);
    mainlayout->setMargin(15);
    mainlayout->setSpacing(10);

    widget=new QWidget();
    this->setCentralWidget(widget);
    Vlayout=new QVBoxLayout(this);
    widget->setLayout(Vlayout);
    Vlayout->setMargin(0);
    Vlayout->setSpacing(0);
    Vlayout->addWidget(imglab);
    Vlayout->addLayout(mainlayout);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);//禁用最大化按钮
    setFixedSize(imglab->width(),imglab->height()+134);     //禁止调节窗口大小


    connect(Ontopbtn,SIGNAL(clicked(bool)),this,SLOT(Ontop()));
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(Select_cont()));
    connect(btn1,SIGNAL(clicked(bool)),this,SLOT(Select_file()));
    connect(btn2,SIGNAL(clicked(bool)),this,SLOT(Ok_btn_onclick()));
    connect(firstedit,SIGNAL(textEdited(QString)),this,SLOT(firstedit_change()));
    connect(submit_btn,SIGNAL(clicked(bool)),this,SLOT(submit_btn_onclick()));
}

void Upload_web::Ontop(){
    if(Isontop){
        Isontop=false;
        Ontopbtn->setText("窗口置顶");
        ::SetWindowPos((HWND)this->winId(),HWND_NOTOPMOST,this->pos().x(),this->pos().y(),width(),height(),SWP_SHOWWINDOW);
    }else {
        Isontop=true;
        Ontopbtn->setText("取消置顶");
        ::SetWindowPos((HWND)this->winId(),HWND_TOPMOST,this->pos().x(),this->pos().y(),width(),height(),SWP_SHOWWINDOW);
    }
}

void Upload_web::Select_cont(){
    QString cont_str=QFileDialog::getExistingDirectory(NULL,"选择目录",".");
    lineedit1->setText(cont_str);
}

void Upload_web::Select_file(){
    QString file_str=QFileDialog::getOpenFileName(NULL,"选择文章",".","*.md");
    lineedit2->setText(file_str);
}

void Upload_web::Ok_btn_onclick(){
    if(Isimgshow){
        QDir dir;
        QFileInfo finfo;
        if(!dir.exists(lineedit1->text())){
            QMessageBox::information(this,"文件夹错误","您选择的文件夹不存在！");
        }else if(!finfo.exists(lineedit2->text())){
            QMessageBox::information(this,"文件错误","未找到您选择的文件！");
        }else{
            QFile file(lineedit2->text());
            file.open(QIODevice::ReadOnly|QIODevice::Text);
            text_ary=file.readAll();
            QString str_tmp=text_ary;
            str_tmp=str_tmp.mid(str_tmp.indexOf("##")+2,150);
            str_tmp.remove(QRegExp("\\s"));
            describe_edit->setText(str_tmp+"...");
            file.close();

            imglab->hide();
            Vlayout->addLayout(secondlayout);
            firstname->show();
            lastname->show();
            describe->show();
            firstedit->show();
            lastedit->show();
            describe_edit->show();
            submit_btn->show();

            lineedit1->setEnabled(false);
            lineedit2->setEnabled(false);
            btn->setEnabled(false);
            btn1->setEnabled(false);

            Isimgshow=false;
            btn2->setText("取消");
        }
    }else {
        imglab->show();

        Vlayout->removeItem(secondlayout);
        firstname->hide();
        lastname->hide();
        describe->hide();
        firstedit->hide();
        lastedit->hide();
        describe_edit->hide();
        submit_btn->hide();

        lineedit1->setEnabled(true);
        lineedit2->setEnabled(true);
        btn->setEnabled(true);
        btn1->setEnabled(true);

        Isimgshow=true;
        btn2->setText("添加");
    }
}

void Upload_web::firstedit_change(){
    if(firstedit->text().trimmed().isEmpty()){
        lastedit->setEnabled(false);
        lastedit->setText("");
    }else {
        lastedit->setEnabled(true);
    }
}

void Upload_web::submit_btn_onclick(){
    //没有命名
    if(firstedit->text().trimmed().isEmpty()){
        QMessageBox::information(this,"错误","您需要为新的文件编辑一个名字，在一级标题栏和二级标题栏填写！");
        return;
    }

    //如果当前名字与文件夹中文件冲突
    if(QFile::exists(lineedit1->text()+"/"+firstedit->text()+lastedit->text()+".md")){
        QMessageBox::information(this,"错误","您命名的文件名已存在，请重试！");
        return;
    }
    //创建此文件
    QFile file_2;
    file_2.setFileName(lineedit1->text()+"/"+firstedit->text()+lastedit->text()+".md");
    file_2.open(QIODevice::WriteOnly|QIODevice::Text);
    file_2.write("");
    file_2.close();

    //当前文件夹中没有articles.json文件
    if(!QFile::exists(lineedit1->text()+"/articles.json")){
        QFile file;
        file.setFileName(lineedit1->text()+"/articles.json");//创建json文件
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        file.write("[]");//初始化
        file.close();
    }

    QFile json_file(lineedit1->text()+"/articles.json");
    if(!json_file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::information(this,"错误","记录文件打开失败！");
        return;
    }
    QByteArray json_data=json_file.readAll();
    json_file.close();

    QJsonParseError json_error;
    QJsonArray json_array;
    QJsonDocument json_doc=QJsonDocument::fromJson(json_data,&json_error);
    //如果没有发生错误
    if(json_error.error==QJsonParseError::NoError){
        //如果包含一个对象
        if(json_doc.isArray()){
//            qDebug()<<"包含数组";
            json_array=json_doc.array();
        }
    }else {
        QMessageBox::information(this,"解析错误","记录文件解析失败，请稍后重试！");
        return;
    }
//    qDebug()<<json_array;
    QJsonArray json_array_new;
    int new_count=0;
    for(int i=0;i<json_array.size();i++){
        QJsonObject ary_obj=json_array[i].toObject();
//        qDebug()<<ary_obj["firstname"];
        QString first_name=ary_obj["firstname"].toString();
        QString last_name=ary_obj["lastname"].toString();
        if(QFile::exists(lineedit1->text()+"/"+first_name+last_name+".md"))
        {
            json_array_new.insert(new_count,QJsonObject(ary_obj));
            new_count++;
        }
    }
    QJsonObject json_obj_new;
    //获取当前系统时间
    QDateTime time=QDateTime::currentDateTime();
    QString time_str=time.toString("yyyyMMddhhmmss");

    json_obj_new.insert("firstname",firstedit->text());
    json_obj_new.insert("lastname",lastedit->text());
    json_obj_new.insert("update",time_str);
    json_obj_new.insert("abstract",describe_edit->toPlainText());
//    qDebug()<<json_obj_new;

    json_array_new.insert(new_count,QJsonObject(json_obj_new));
//    qDebug()<<json_array_new;

    QJsonDocument new_json_doc;
    new_json_doc.setArray(json_array_new);
    QByteArray new_json_data=new_json_doc.toJson(QJsonDocument::Compact);

    QFile json_file_new(lineedit1->text()+"/articles.json");
    if(!json_file_new.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::information(this,"错误","记录文件打开失败！");
        return;
    }
    json_file_new.write(new_json_data);
    json_file_new.close();
    QMessageBox::information(this,"成功","添加成功！");

    file_2.open(QIODevice::WriteOnly|QIODevice::Text);
    file_2.write(text_ary);
    file_2.close();
}

Upload_web::~Upload_web()
{

}
