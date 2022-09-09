#include"QCalculation.h"

cal_movefunc::cal_movefunc()
{

}
int cal_movefunc::func_wave(vector<vector<double>>data,double *&t,double* &sg, double& Amp3,double& Amp10,double&t0,int n)
{
        int m;
        double detT,nn,spectrumArea,m2,m4,xisu,meang,sum;
        t = new double[n];
        double *g = new double[n];

        sum=0.0;

        //给数组t赋值
        for (int i=0;i<n;i++)
        {
            t[i]=data[i][0];
        }
        //给数组g赋值
        for (int i=0;i<n;i++)
        {
            g[i]=data[i][1];
        }

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
        detT = abs(t[1]-t[2]);//时间间隔

        nn=n;

        m=int(ceil((nn/30)));//滞后的最大数目=取最小整数


        double* RR = new double[m+1];
        sg = new double[m+1];
        double* sg0 = new double[m+1];
        double* w = new double[m+1];
        for (int i=0; i<m+1;i++)
        {
            RR[i]=0;
        }
        for (int i=0; i<m+1;i++)
        {
            sg[i]=0;
        }
        for (int i=0; i<m+1;i++)
        {
            sg0[i]=0;
        }
        for (int i=0; i<m+1;i++)
        {
            w[i]=0;
        }

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
            cout<<sg[i]<<endl;
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
         double min_num =0;
        for(int i=0 ;i<m-1;i++)
        {
            if(min_num>sg[i])
            min_num=sg[i];
        }
        t0=0.0;
        spectrumArea=0.0,m2=0.0,m4=0.0;
        for (int i=0;i<m;i++)
        {
            spectrumArea +=abs(w[i+1]-w[i])*(sg[i+1]+sg[i])/2;
            m2+=abs(w[i+1]-w[i])*(sg[i+1]*pow(w[i+1],2)+sg[i]*pow(w[i],2))/2;
            m4+=abs(w[i+1]-w[i])*(sg[i+1]*pow(w[i+1],4)+sg[i]*pow(w[i],4))/2;

        }
        double ww=sqrt(m2/spectrumArea);
        t0=2*PI/ww;
        xisu = 1-pow(m2,2)/spectrumArea/m4;
        Amp3=sqrt(spectrumArea*(1-xisu/2))*2;
        Amp10=sqrt(spectrumArea*(1-xisu/2))*2.55;
        return 0;

}

void cal_movefunc::func_control(double L, double V, double t1, double t2, double te, double t3, double t4, double te1, double t5, double t6, double te2, double t7, double a, double pe, double pe1, double pe2, double b0, double b2, double b4, double b6, double& K, double& T)
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

void cal_movefunc::func_rotation(double V, double D, double b)
{
    D=58.9*V/b;
}
