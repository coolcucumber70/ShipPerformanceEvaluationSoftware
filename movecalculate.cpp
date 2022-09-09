#include "movecalculate.h"
#define G 9.8
MoveCalculate::MoveCalculate(QObject *parent) : QObject(parent)
{

}
QVector<QVector<float>> MoveCalculate::MoveResult={{1,2},{3,4},{5,6},{7,8},{9,10},{11,12},{13,14}};

QVector<double> MoveCalculate::getsqldata(QString sql)
{
    QVector<double>data;
    QSqlQuery query=glSqlDatabse.ExecuteQuery(sql);
    while(query.next())
        {

            data.append(query.value(0).toDouble());
        }
    return data;

}

void MoveCalculate::workstart()
{
    QVector<QVector<double>>source;
    QVector<QVector<double>>result;
    QList<QString>sql=QList<QString>()<<"SELECT data1 FROM tb_move ORDER BY id DESC LIMIT 0,1000"
                                     <<"SELECT data2 FROM tb_move ORDER BY id DESC LIMIT 0,1000"
                                    <<"SELECT data3 FROM tb_move ORDER BY id DESC LIMIT 0,1000"
                                   <<"SELECT data1 FROM tb_acceleration ORDER BY id DESC LIMIT 0,1000"
                                  <<"SELECT data2 FROM tb_acceleration ORDER BY id DESC LIMIT 0,1000"
                                 <<"SELECT data3 FROM tb_acceleration ORDER BY id DESC LIMIT 0,1000"
                                <<"SELECT data FROM tb_wave ORDER BY id DESC LIMIT 0,1000";
    for(auto str:sql){
        QVector<double>v=getsqldata(str);
        source.append(v);
    }
    for(int i=0;i<source.size()-1;i++){

        double Amp3,MAX;
        QVector<double>v=func_move(source[i], Amp3, MAX);
        result.append(v);
    }
    //result写入数据库
    //计算出静态数组moveresult
    //moveresult写入数据库
    emit sendresult();//发射信号告诉结果出来了

}

void MoveCalculate::preworkstart(double h13,double T1)
{
     QVector<QVector<float>> preMoveResult;
     //getsqldata(QString sql)//写入数据库中的六组数据
             //写一个for循环显示出来

}
void MoveCalculate::func_control(double L, double V, double t1, double t2, double te, double t3, double t4, double te1, double t5, double t6, double te2, double t7, double a, double pe, double pe1, double pe2, double b0, double b2, double b4, double b6, double& K, double& T)
{
    double A=0.5*a*(-t1+2*t2+2*t3-2*t4-2*t5+2*te1);

    double B=0.5*a*(-t1+2*t2+2*t3-2*t4-2*t5+2*t6+2*t7-2*te2);

    double C=0.5*a*(-t1+2*t2+2*t3-2*te);

    double D=a*(t3-te);

    double E=a*(te1-t5);

    double F=a*(t7-te2);

    double K68=(te2*pe1-te1*pe2)/(A*te2-B*te1);

    double ar=(pe1-K68*A)/(te1*K68);

    double K4=pe/(C+ar*te);

    double T4=(K4*D+K4*ar*(te-t2)+a-pe)/(b0-b2);

    double T6=(K68*E+K68*ar*(te1-t4)-a-pe1)/(b0-b4);

    double T8=(K68*F+K68*ar*(te2-t6)+a-pe2)/(b0-b6);

    double T68=0.5*(T6+T8);

    double k=0.5*(K4+K68);

    double t=0.5*(T4+T68);
    K=L*k/V;
    T=V*t/L;

}
//姿态与运动波能谱
QVector<double> MoveCalculate::func_move(QVector<double> g, double &Amp3, double &MAX)
{
    int n=g.size();
    int m;
    double nn,spectrumArea,m2,m4,xisu,meang,sum;
    sum=0.0;
    for (int i=0;i<n;i++)
    {
        sum+=g[i];
    }
    meang= sum/n;//求平均值

    //修正
    for (int i=0;i<n;i++)
    {
        g[i]=g[i]-meang;
    }


    nn=n;

    m=int(ceil((nn/30)));//滞后的最大数目=取最小整数


    QVector<double>RR(m+1,0);
    QVector<double>sg(m+1,0);
    QVector<double>sg0(m+1,0);
    QVector<double>w(m+1,0);

    int nn1=int(nn);
    for (int k=0;k<m+1;k++)
    {
        RR[k]+=g[0]*g[k];
        RR[k]+=g[nn1-k-1]*g[nn1-1];
        for (int i=1;i<nn1-k-1;i++)
        {
            RR[k]+=2*g[i]*g[i+k];
        }
        RR[k]=RR[k]/(2*(nn1-1));
    };


    for (int i=0;i<=m;i++)
    {
        for (int k=0;k<m-1;k++)
        {
            sg0[i]+=RR[k+1]*cos(i*(k+1)*PI/m);
        }
        sg0[i]*=2;
        sg0[i]+=RR[0]+RR[m]*cos(i*PI);
        sg0[i]*=detT/PI;
        w[i]=i*PI/(detT*m);
    }
    sg[0]=sg0[0]*0.54+sg0[1]*0.46;//海明平滑30页
    sg[m]=sg0[m-1]*0.46+sg0[m]*0.54;//海明平滑30页
    for (int i=0;i<m-1;i++)
    {
        sg[i+1]=(sg0[i]+sg0[i+2])*0.23+sg0[i+1]*0.54;
    }//海明平滑30页
    for (int i=0;i<m-1;i++)
    {
        //cout<<sg[i]<<endl;
        if (abs(sg[i])<0.00001)
        {
            sg[i]=0;
        }
    }//把小于-5次方的数替换成0
    double max_num =0;
    for(int i=0 ;i<m-1;i++)
    {
        if(max_num<sg[i])
            max_num=sg[i];
    }
    spectrumArea=0.0,m2=0.0,m4=0.0;
    for (int i=0;i<m;i++)
    {
        spectrumArea +=abs(w[i+1]-w[i])*(sg[i+1]+sg[i])/2;
        m2+=abs(w[i+1]-w[i])*(sg[i+1]*pow(w[i+1],2)+sg[i]*pow(w[i],2))/2;
        m4+=abs(w[i+1]-w[i])*(sg[i+1]*pow(w[i+1],4)+sg[i]*pow(w[i],4))/2;

    }
    xisu = 1-pow(m2,2)/spectrumArea/m4;
    Amp3=sqrt(spectrumArea*(1-xisu/2))*2;
    MAX = max_num;
    return sg0;
}
//海浪波能谱
QVector<double> MoveCalculate::func_wave(QVector<double> g, double &Amp3, double &t0)
{
    int n=g.size();
    int m;
    double nn,spectrumArea,m2,m4,xisu,meang,sum;
    sum=0.0;
    for (int i=0;i<n;i++)
    {
        sum+=g[i];
    }
    meang= sum/n;//求平均值

    //修正
    for (int i=0;i<n;i++)
    {
        g[i]=g[i]-meang;
    }


    nn=n;

    m=int(ceil((nn/30)));//滞后的最大数目=取最小整数


    QVector<double>RR(m+1,0);
    QVector<double>sg(m+1,0);
    QVector<double>sg0(m+1,0);
    QVector<double>w(m+1,0);

    int nn1=int(nn);
    for (int k=0;k<m+1;k++)
    {
        RR[k]+=g[0]*g[k];
        RR[k]+=g[nn1-k-1]*g[nn1-1];
        for (int i=1;i<nn1-k-1;i++)
        {
            RR[k]+=2*g[i]*g[i+k];
        }
        RR[k]=RR[k]/(2*(nn1-1));
    };


    for (int i=0;i<=m;i++)
    {
        for (int k=0;k<m-1;k++)
        {
            sg0[i]+=RR[k+1]*cos(i*(k+1)*PI/m);
        }
        sg0[i]*=2;
        sg0[i]+=RR[0]+RR[m]*cos(i*PI);
        sg0[i]*=detT/PI;
        w[i]=i*PI/(detT*m);
    }
    sg[0]=sg0[0]*0.54+sg0[1]*0.46;//海明平滑30页
    sg[m]=sg0[m-1]*0.46+sg0[m]*0.54;//海明平滑30页
    for (int i=0;i<m-1;i++)
    {
        sg[i+1]=(sg0[i]+sg0[i+2])*0.23+sg0[i+1]*0.54;
    }//海明平滑30页
    for (int i=0;i<m-1;i++)
    {
        //cout<<sg[i]<<endl;
        if (abs(sg[i])<0.00001)
        {
            sg[i]=0;
        }
    }//把小于-5次方的数替换成0
    double max_num =0;
    for(int i=0 ;i<m-1;i++)
    {
        if(max_num<sg[i])
            max_num=sg[i];
    }
    spectrumArea=0.0,m2=0.0,m4=0.0;
    for (int i=0;i<m;i++)
    {
        spectrumArea +=abs(w[i+1]-w[i])*(sg[i+1]+sg[i])/2;
        m2+=abs(w[i+1]-w[i])*(sg[i+1]*pow(w[i+1],2)+sg[i]*pow(w[i],2))/2;
        m4+=abs(w[i+1]-w[i])*(sg[i+1]*pow(w[i+1],4)+sg[i]*pow(w[i],4))/2;

    }
    double ww=sqrt(m2/spectrumArea);
    xisu = 1-pow(m2,2)/spectrumArea/m4;
    Amp3=sqrt(spectrumArea*(1-xisu/2))*2;
    t0=2*PI/ww;
    return sg0;
}
//预报方法
void MoveCalculate::func_rotation(double V, double& D, double b)
{
    D=58.9*V/b;
}

QVector<double> MoveCalculate::func_prediction(double h13,double T1,double h131,double T11,QVector<double>&w,QVector<double>&Sg1)
{
    QVector<double>result;
    int n=w.size();
    QVector<double>Sg,Sg2,Sg3;//生成计算运动响应函数的波能谱，用Sg表示
    QVector<double>RAO;
    RAO.resize(n);
    Sg.resize(n);
    Sg2.resize(n);
    Sg3.resize(n);
    double A1,B1;
    A1=173*pow(h131,2)/pow(T11,4);
    B1=691/pow(T11,4);
    for (int i=0;i<n;i++)//给数组Sg赋值——此处可以改写成调取经过相关函数法计算后的波浪谱数据
    {
        Sg[i]=(A1/pow(w[i],5))*exp(-B1/pow(w[i],4));;
    }

    //计算运动响应函数////////////////////////////////////////////////////
    for (int i=0;i<n;i++)//给数组RAO赋值，
    {
        RAO[i]=Sg1[i]/Sg[i];//需要计算的

    }

    //生成理论双参数谱
    double A,B;
    A=173*pow(h13,2)/pow(T1,4);
    B=691/pow(T1,4);//ITTC双参数谱参数A,B

    for (int i=0;i<n;i++)
    {
        Sg2[i]=(A/pow(w[i],5))*exp(-B/pow(w[i],4));//此处生成双参数谱

    }
    //得到预报谱（运动或加速度）

    for (int i=0;i<n;i++)
    {
        Sg3[i]=Sg2[i]*RAO[i];//使用理论谱与计算出来的RAO进行相乘得出预报出来的运动响应

    }

    double 	m0=0.0,m2=0.0,m4=0.0;

    for (int i=0;i<n;i++)
    {
        m0 +=abs(w[i+1]-w[i])*(Sg3[i+1]+Sg3[i])/2;
        m2+=abs(w[i+1]-w[i])*(Sg3[i+1]*pow(w[i+1],2)+Sg3[i]*pow(w[i],2))/2;
        m4+=abs(w[i+1]-w[i])*(Sg3[i+1]*pow(w[i+1],4)+Sg3[i]*pow(w[i],4))/2;//根据耐波性书公式进行特征参数计算，与算运动和加速度处的程序类似
    }

    double xisu = 1-pow(m2,2)/m0/m4;
    double Amp13=sqrt(m0*(1-xisu/2))*2;
    double Amp110=sqrt(m0*(1-xisu/2))*2.55;//需要显示的三分之一有义值与十分之一最大值
    result.push_back(Amp13);
    result.push_back(Amp110);


    //生成理论单参数谱
    A=8.1*pow(G,2);
    B=3.11/pow(h13,2);//ITTC单参数谱参数A,B

    for (int i=0;i<n;i++)
    {
        Sg2[i]=(A/pow(w[i],5))*exp(-B/pow(w[i],4));//此处生成单参数谱，与双参谱公式相同只是A、B的计算方式不同

    }
    //得到预报谱（运动或加速度）

    for (int i=0;i<n;i++)
    {
        Sg3[i]=Sg2[i]*RAO[i];//使用理论谱与计算出来的RAO进行相乘得出预报出来的运动响应

    }

    m0=0.0,m2=0.0,m4=0.0;

    for (int i=0;i<n;i++)
    {
        m0 +=abs(w[i+1]-w[i])*(Sg3[i+1]+Sg3[i])/2;
        m2+=abs(w[i+1]-w[i])*(Sg3[i+1]*pow(w[i+1],2)+Sg3[i]*pow(w[i],2))/2;
        m4+=abs(w[i+1]-w[i])*(Sg3[i+1]*pow(w[i+1],4)+Sg3[i]*pow(w[i],4))/2;//根据耐波性书公式进行特征参数计算，与算运动和加速度处的程序类似
    }

    xisu = 1-pow(m2,2)/m0/m4;
    Amp13=sqrt(m0*(1-xisu/2))*2;
    Amp110=sqrt(m0*(1-xisu/2))*2.55;//需要显示的三分之一有义值与十分之一最大值
    result.push_back(Amp13);
    result.push_back(Amp110);

    //生成理论中国沿海谱
    double C;

    C=6.28*sqrt(h13);//中国沿海谱参数C

    for (int i=0;i<n;i++)
    {
        Sg2[i]=(0.74/pow(w[i],5))*exp(-pow(G,2)/(pow(C,2)*pow(w[i],2)));//此处生成中国沿海谱

    }
    //得到预报谱（运动或加速度）

    for (int i=0;i<n;i++)
    {
        Sg3[i]=Sg2[i]*RAO[i];//使用理论谱与计算出来的RAO进行相乘得出预报出来的运动响应

    }

    m0=0.0,m2=0.0,m4=0.0;

    for (int i=0;i<n;i++)
    {
        m0 +=abs(w[i+1]-w[i])*(Sg3[i+1]+Sg3[i])/2;
        m2+=abs(w[i+1]-w[i])*(Sg3[i+1]*pow(w[i+1],2)+Sg3[i]*pow(w[i],2))/2;
        m4+=abs(w[i+1]-w[i])*(Sg3[i+1]*pow(w[i+1],4)+Sg3[i]*pow(w[i],4))/2;//根据耐波性书公式进行特征参数计算，与算运动和加速度处的程序类似
    }

     xisu = 1-pow(m2,2)/m0/m4;
    Amp13=sqrt(m0*(1-xisu/2))*2;
    Amp110=sqrt(m0*(1-xisu/2))*2.55;//需要显示的三分之一有义值与十分之一最大值
    result.push_back(Amp13);
    result.push_back(Amp110);
    return result;


}
