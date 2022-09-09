#ifndef ASSESSMENTFORM_H
#define ASSESSMENTFORM_H

#include <QWidget>
#include<QFile>
#include<QTextStream>
namespace Ui {
class AssessmentForm;
}

class AssessmentForm : public QWidget
{
    Q_OBJECT

public:
    explicit AssessmentForm(QWidget *parent = nullptr);
    ~AssessmentForm();
    double AssessmentCal();
    bool get_data();
    bool save_data();

private slots:
    void on_calBtn_clicked();

    void on_InquireBtn_clicked();

    void on_saveBtn_clicked();

private:
    double m1[6][5];
    double m2[5][5];
    double m3[3][5];
    double m4[5][5];
    double m5[3][5];
    double m6[4][5];
    Ui::AssessmentForm *ui;
};

#endif // ASSESSMENTFORM_H
