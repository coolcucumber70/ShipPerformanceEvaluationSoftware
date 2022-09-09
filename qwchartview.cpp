#include "qwchartview.h"

#include    <QChartView>
#include<QCoreApplication>
#include<QGraphicsLayout>
#include <QtCharts/QLegendMarker>

void QWChartView::mousePressEvent(QMouseEvent *event)
{//鼠标左键按下，记录beginPoint
    if (event->button()==Qt::LeftButton)
        beginPoint=event->pos();
    QChartView::mousePressEvent(event);
}

void QWChartView::mouseMoveEvent(QMouseEvent *event)
{//鼠标移动事件
    QPoint  point;
    point=event->pos();

    emit mouseMovePoint(point);
    QChartView::mouseMoveEvent(event);
}

void QWChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    { //鼠标左键释放，获取矩形框的endPoint,进行缩放
        endPoint=event->pos();
        QRectF  rectF;
        rectF.setTopLeft(this->beginPoint);
        rectF.setBottomRight(this->endPoint);
        chart()->zoomIn(rectF);
    }
    else if (event->button()==Qt::RightButton)
        chart()->zoomReset(); //鼠标右键释放，resetZoom
    QChartView::mouseReleaseEvent(event);
}

void QWChartView::keyPressEvent(QKeyEvent *event)
{//按键控制
    switch (event->key()) {
    case Qt::Key_Plus:  //+
        chart()->zoom(1.2);
        break;
    case Qt::Key_Minus:
        chart()->zoom(0.8);
        break;
    case Qt::Key_Left:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, -10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_PageUp:
        chart()->scroll(0, 50);
        break;
    case Qt::Key_PageDown:
        chart()->scroll(0, -50);
        break;
    case Qt::Key_Home:
        chart()->zoomReset();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
//    QGraphicsView::keyPressEvent(event);
}

QWChartView::QWChartView(QWidget *parent,QString title):QChartView(parent)
{
    this->setDragMode(QGraphicsView::RubberBandDrag);
//    this->setRubberBand(QChartView::RectangleRubberBand);//设置为矩形选择方式
//    this->setRubberBand(QChartView::VerticalRubberBand);
//    this->setRubberBand(QChartView::HorizontalRubberBand);
    this->setMouseTracking(true);//必须开启此功能
    setCursor(QCursor(Qt::PointingHandCursor)); //设置鼠标指针为手指形
    QPen penY(Qt::darkBlue,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    Chart = new QChart();
    axisX = new QValueAxis(this);
    axisY = new QValueAxis(this);
    Chart->setContentsMargins(0, 0, 0, 0);//设置外边界全部为0
    Chart->layout()->setContentsMargins(0,0,0,0);
    Chart->setBackgroundRoundness(0);
    //chart->setMargins(QMargins(0, 0, 0, 0));//设置内边界全部为0
    QFont font = Chart->legend()->font();
    font.setPointSizeF(12);
    Chart->legend()->setFont(font);
    Chart->legend()->setAlignment(Qt::AlignTop);
    Chart->legend()->detachFromChart();
    //chart->legend()->setBackgroundVisible(true);
    //chart->legend()->setGeometry(QRect(100,50,0,0));
    //chart->legend()->attachToChart();
    //chart->legend()->hide(); //隐藏图例
    //series->setName("极大值：0 有义值：0");
    QSplineSeries *series=new QSplineSeries(this);
    Chart->addSeries(series);
    axisX->setTickCount(5);                             //设置坐标轴格数
    axisY->setTickCount(5);
    axisX->setRange(0, 10);
    axisY->setRange(-2, 2);
    //axisX->setTitleText(titlex);
    axisX->setTitleVisible(false);
    axisX->setGridLineVisible(false);
    axisY->setLinePenColor(QColor(Qt::darkBlue));        //设置坐标轴颜色样式
    axisY->setGridLineColor(QColor(Qt::darkBlue));
    axisY->setGridLineVisible(false);
    //axisY->setTitleText(titley);
    axisY->setTitleVisible(false);
    axisY->setLinePen(penY);
    axisX->setLinePen(penY);
    Chart->addAxis(axisX,Qt::AlignBottom);               //设置坐标轴位于chart中的位置
    Chart->addAxis(axisY,Qt::AlignLeft);//把线添加到chart
    Chart->setTitle(title);
    series->attachAxis(axisX);                           //把数据添加到坐标轴上
    series->attachAxis(axisY);
    setChart(Chart);
}

QWChartView::~QWChartView()
{

}
