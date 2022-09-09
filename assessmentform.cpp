#include "assessmentform.h"
#include "ui_assessmentform.h"
#include<iostream>
#include<iomanip>
#include<qdebug.h>
using namespace std;
AssessmentForm::AssessmentForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AssessmentForm)
{
    ui->setupUi(this);

}

AssessmentForm::~AssessmentForm()
{
    delete ui;
}

double AssessmentForm::AssessmentCal()
{
    double arr0[6] = { 0.06774146,0.13150799,0.13238521,0.0243489,0.3491171,0.29489934 };
    double arr1[6] = { 0.33325923,0.16791548,0.24419985,0.12709394,0.04388487,0.08364663 };
    double arr2[5] = { 0.13865774,0.06989616,0.18613246,0.3507084,0.25460523 };
    double arr3[3] = { 0.07192743,0.27895457,0.649118 };
    double arr4[5] = { 0.14905884,0.28960113,0.41870547,0.09139847,0.05123609 };
    double arr5[3] = { 0.24931053,0.15705579,0.59363369 };
    double arr6[4] = { 0.13386172,0.51028683,0.28780379,0.06804765 };
    double score[5] = { 1,3,5,7,9 };

    //输入隶属度矩阵
    double M1[5] = { 0 };
    double M2[5] = { 0 };
    double M3[5] = { 0 };
    double M4[5] = { 0 };
    double M5[5] = { 0 };
    double M6[5] = { 0 };
    double M0[6][5] = { 0 };
    double W0[5] = { 0 };
    double S = { 0 };

    for (int k = 0; k < 5; k++)
    {
        for (int j = 0; j < 6; j++)
        {
            M1[k] = M1[k] + arr1[j] * m1[j][k];
        }

    }
        for (int i = 0; i < 5; i++)
        {
            cout << "  " << M1[i];
        }
        cout << endl;

    for (int k = 0; k < 5; k++)
    {
        for (int j = 0; j < 5; j++)
        {
                M2[k] = M2[k] + arr2[j] * m2[j][k];
         }

    }
        for (int i = 0; i < 5; i++)
        {
            cout << "  " << M2[i];
        }
        cout << endl;

    for (int k = 0; k < 5; k++)
    {
        for (int j = 0; j < 3; j++)
         {
                M3[k] = M3[k] + arr3[j] * m3[j][k];
         }

    }
        for (int i = 0; i < 5; i++)
        {
            cout << "  " << M3[i];
        }
        cout << endl;

    for (int k = 0; k < 5; k++)
    {
            for (int j = 0; j < 5; j++)
            {
                M4[k] = M4[k] + arr4[j] * m4[j][k];
            }

    }
        for (int i = 0; i < 5; i++)
        {
            cout << "  " << M4[i];
        }
        cout << endl;

    for (int k = 0; k < 5; k++)
    {
            for (int j = 0; j < 3; j++)
            {
                M5[k] = M5[k] + arr5[j] * m5[j][k];
            }

    }
        for (int i = 0; i < 5; i++)
        {
            cout << "  " << M5[i]<<endl;
        }


    for (int k = 0; k < 5; k++)
    {
            for (int j = 0; j < 4; j++)
            {
                M6[k] = M6[k] + arr6[j] * m6[j][k];
            }

    }
        for (int i = 0; i < 5; i++)
        {
            cout << "  " << M6[i];
        }

        for (int j = 0; j < 5; j++)
        {
               M0[0][j] = M0[0][j] + M1[j];
        }
              cout << endl;
        for (int j = 0; j < 5; j++)
        {
               M0[1][j] = M0[1][j] + M2[j];
        }
              cout << endl;

        for (int j = 0; j < 5; j++)
        {
                  M0[2][j] = M0[2][j] + M3[j];
        }
              cout << endl;

        for (int j = 0; j < 5; j++)
        {
                  M0[3][j] = M0[3][j] + M4[j];
        }
              cout << endl;

        for (int j = 0; j < 5; j++)
        {
                  M0[4][j] = M0[4][j] + M5[j];
        }
              cout << endl;

        for (int j = 0; j < 5; j++)
        {
                  M0[5][j] = M0[5][j] + M6[j];
        }
              cout << endl;

        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 5; j++) {
                cout << " " << M0[i][j];
            }
                cout << endl;
        }

        for (int k = 0; k < 5; k++)
        {
            for (int j = 0; j < 6; j++)
            {
                W0[k] = W0[k] + arr0[j] * M0[j][k];
            }
        }
        for (int i = 0; i < 5; i++)
        {
            cout << " " << W0[i];
        }

        for (int i = 0; i < 5; i++)
        {
            S = S + score[i] * W0[i];
        }
        return S;
        cout<<"the result is :"<<S<<endl;
}

bool AssessmentForm::get_data()
{
    QString aFileName="D:/code_learning/qt_pro/ruajian2/M_array.txt";
    QStringList fFileContent;//文件内容字符串列表
    QFile aFile(aFileName);  //以文件方式读出
    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读文本方式打开文件
    {
        qDebug()<<"读取数据成功了";
        QTextStream aStream(&aFile); //用文本流读取文件
        //ui->plainTextEdit->clear();//清空
        while (!aStream.atEnd())
        {
            QString str=aStream.readLine();//读取文件的一行
            //ui->plainTextEdit->appendPlainText(str); //添加到文本框显示
            fFileContent.append(str); //添加到 StringList
        }
    }
    else{
        return false;
    }
    aFile.close();//关闭文件
    QString aLineText=fFileContent.at(0); //获取 数据区 的一行\\s+
    QStringList tmpList=aLineText.split(',',QString::SkipEmptyParts);

    for (int i=0;i<6;i++)
        {
            for(int j=0;j<5;j++){
                m1[i][j]=tmpList[i*5+j].toDouble();
                ui->tableWidget->setItem(i, j, new QTableWidgetItem(tmpList[i*5+j]));
            }
        }
    aLineText=fFileContent.at(1); //获取 数据区 的一行
    tmpList=aLineText.split(',',QString::SkipEmptyParts);
    for (int i=0;i<5;i++)
        {
            for(int j=0;j<5;j++){
                m2[i][j]=tmpList[i*5+j].toDouble();
                //ui->tableWidget_1->setItem(i, j, new QTableWidgetItem(tmpList[i*5+j]));
            }
        }
    aLineText=fFileContent.at(2); //获取 数据区 的一行
    tmpList=aLineText.split(',',QString::SkipEmptyParts);
    for (int i=0;i<3;i++)
        {
            for(int j=0;j<5;j++){
                m3[i][j]=tmpList[i*5+j].toDouble();
                //ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(tmpList[i*5+j]));
            }
        }
    aLineText=fFileContent.at(3); //获取 数据区 的一行
    tmpList=aLineText.split(',',QString::SkipEmptyParts);
    for (int i=0;i<5;i++)
        {
            for(int j=0;j<5;j++){
                m4[i][j]=tmpList[i*5+j].toDouble();
                ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(tmpList[i*5+j]));
            }
        }
    aLineText=fFileContent.at(4); //获取 数据区 的一行
    tmpList=aLineText.split(',',QString::SkipEmptyParts);
    for (int i=0;i<3;i++)
        {
            for(int j=0;j<5;j++){
                m5[i][j]=tmpList[i*5+j].toDouble();
                ui->tableWidget_4->setItem(i, j, new QTableWidgetItem(tmpList[i*5+j]));
            }
        }
    aLineText=fFileContent.at(5); //获取 数据区 的一行
    tmpList=aLineText.split(',',QString::SkipEmptyParts);
    for (int i=0;i<4;i++)
        {
            for(int j=0;j<5;j++){
                ui->tableWidget_5->setItem(i, j, new QTableWidgetItem(tmpList[i*5+j]));
                m6[i][j]=tmpList[i*5+j].toDouble();
            }
        }
    return true;


}

bool AssessmentForm::save_data()
{

}

void AssessmentForm::on_calBtn_clicked()
{
    double result=AssessmentCal();
    ui->doubleSpinBox->setValue(result);
}

void AssessmentForm::on_InquireBtn_clicked()
{
    get_data();
}

void AssessmentForm::on_saveBtn_clicked()
{


}
