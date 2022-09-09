#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m=new moveform(this);
    ui->tabWidget->insertTab(1,m,"环境与运动模块");
    s=new structureform(this);
    ui->tabWidget->insertTab(2,s,"结构检测模块");
    //l=new LogForm(this);
    //ui->tabWidget->insertTab(0,l,"登录界面");
    a=new AssessmentForm(this);
    ui->tabWidget->insertTab(3,a,"综合评估模块");
    setWindowState(Qt::WindowMaximized);

}

Widget::~Widget()
{
    delete ui;
}

