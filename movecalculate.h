#ifndef MOVECALCULATE_H
#define MOVECALCULATE_H

#include <QObject>
#include "sqlwork/CSQLDatabase.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<ctime>
#include<algorithm>
#include<math.h>
#include<QVector>

#define PI 3.14159265
#define detT 0.02
using namespace std;
class MoveCalculate : public QObject
{
    Q_OBJECT
public:
    explicit MoveCalculate(QObject *parent = nullptr);

    static QVector<QVector<float>> MoveResult;//定义静态变量，用来映射到table中

    static void func_control(double L, double V, double t1, double t2, double te, double t3, double t4, double te1, double t5, double t6, double te2, double t7, double a, double pe, double pe1, double pe2, double b0, double b2, double b4, double b6, double& K, double& T);
signals:
    void sendresult();
    void sendpreresult();
private slots:
    void workstart();
    void preworkstart(double h13,double T1);

private:
    QVector<double>getsqldata(QString sql);
    void func_rotation(double V,double& D,double b);
    QVector<double>  func_prediction(double h13,double T1,double h131,double T11,QVector<double>&w,QVector<double>&Sg1);
    QVector<double> func_move(QVector<double> g,double &Amp3,double &MAX);
    QVector<double> func_wave(QVector<double> g,double &Amp3,double &t0);
};

#endif // MOVECALCULATE_H
