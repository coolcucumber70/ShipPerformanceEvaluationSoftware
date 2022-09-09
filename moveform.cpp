#include "moveform.h"
#include "ui_moveform.h"
#include<ChartView.h>
#include<QTreeWidgetItem>
moveform::moveform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::moveform)
{
    ui->setupUi(this);
    createform();
    cal= new MoveCalculate;
    QThread *Thread3 = new QThread;
    cal->moveToThread(Thread3);
    Thread3->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    connect(timer1,SIGNAL(timeout()),cal,SLOT(workstart()));
    timer->start(1000);
    //timer1->start(10000);
    treeWidgetUpdate();


    setWindowState(Qt::WindowMaximized);
    //connect(cal,&MoveCalculate::sendresult,this,&moveform::resultUpdate);

}

moveform::~moveform()
{
    delete ui;
}

void moveform::createform()
{
     ui->qTime->setDigitCount(25);

}

void moveform::getinfor(QString qtime, double qspeed, QString qwind, double qwave)
{
    ui->qSpeed->setText(QString::number(qspeed, 'f', 2)+" m/s");
    //ui->qTime->display(qtime);
    ui->qWind->setText(qwind);
    ui->qWave->setText(QString::number(qwave, 'f', 2)+" m/s");
}

void moveform::timerUpdate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->qTime->display(str);
}

void moveform::treeWidgetUpdate()
{
    QVector<QVector<float>>result=MoveCalculate::MoveResult;
    QTreeWidgetItem*topItem0=ui->treeWidget->topLevelItem(0);
    QTreeWidgetItem*topItem1=ui->treeWidget->topLevelItem(1);
    QTreeWidgetItem*topItem2=ui->treeWidget->topLevelItem(2);
    for(int i=0;i<3;i++){
         QTreeWidgetItem*child0=topItem0->child(i);
         QTreeWidgetItem*child1=topItem1->child(i);
         child0->setText(1,QString::number(result[i][0],'f',2));
         child0->setText(2,QString::number(result[i][1],'f',2));
         child1->setText(1,QString::number(result[i+3][0],'f',2));
         child1->setText(2,QString::number(result[i+3][1],'f',2));
    }
    QTreeWidgetItem*child0=topItem2->child(0);
    child0->setText(1,QString::number(result[6][0],'f',2));
    child0->setText(2,QString::number(result[6][1],'f',2));


}

void moveform::chartViewUpdate(QVector<float> data)
{
    if(premovetype!=movetype){
        ui->moveView->buffer.clear();
        premovetype=movetype;
    }
    ui->moveView->updateseries(data);
}

void moveform::on_drawBtn_clicked()
{
    QString sql="SELECT time1,angle1,time2,angle2 FROM tb_control`";
    QSqlQuery query=glSqlDatabse.ExecuteQuery(sql);
    QVector<QPointF>v1;
    QVector<QPointF>v2;
    while(query.next())
    {
        v1.append(QPointF(query.value(0).toDouble(),query.value(1).toDouble()));
        v2.append(QPointF(query.value(2).toDouble(),query.value(3).toDouble()));
    }
    //ui->RudderView->updateseries(v1,0);
    //ui->RudderView->updateseries(v2,1);
}

void moveform::on_calBtn_clicked()
{
    double K,T;
    double a =2;
    double L=1,V=2;
    MoveCalculate::func_control(L, V, ui->t1->text().toDouble(), ui->t2->text().toDouble(), ui->te->text().toDouble(),ui->t3->text().toDouble(), ui->t4->text().toDouble(), ui->te1->text().toDouble(),ui->t5->text().toDouble(), ui->t6->text().toDouble(),ui->te2->text().toDouble(),ui->t7->text().toDouble(),a, ui->pe->text().toDouble(),ui->pe1->text().toDouble(),ui->pe2->text().toDouble(), ui->b0->text().toDouble(),ui->b2->text().toDouble(),ui->b4->text().toDouble(),ui->b6->text().toDouble(), K, T);
    ui->qMvalue->setText(QString::number(K,'f',2));
    ui->qRotation->setText(QString::number(L,'f',2));
}

void moveform::on_CalBtn_clicked()
{


}

void moveform::on_DrawBtn_clicked()
{
    QString sql="SELECT x,y FROM tb_control`";
    QSqlQuery query=glSqlDatabse.ExecuteQuery(sql);
    QVector<QPointF>v;
    while(query.next())
    {
        v.append(QPointF(query.value(0).toDouble(),query.value(1).toDouble()));

    }

    ui->RotationView->updateseries(v);
}
MOVETYPE moveform::movetype=None;

void moveform::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if(current->parent()!=NULL)
    {
        if(current==ui->treeWidget->topLevelItem(0)->child(0)){
            movetype=Roll;}
        else if(current==ui->treeWidget->topLevelItem(0)->child(1)){
            movetype=Pitching;}
        else if(current==ui->treeWidget->topLevelItem(0)->child(1)){
            movetype=Heave;}
        else if(current==ui->treeWidget->topLevelItem(1)->child(0)){
            movetype=Front;}
        else if(current==ui->treeWidget->topLevelItem(1)->child(1))
            movetype=Middle;
        else if(current==ui->treeWidget->topLevelItem(1)->child(2))
            movetype=Rear;
        else{
            movetype=Wave;
        }

    }


}
