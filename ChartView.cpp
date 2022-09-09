#include "ChartView.h"
#include <QApplication>
#include <QValueAxis>
#include<QCoreApplication>
#include<QGraphicsLayout>
#include <QtCharts/QLegendMarker>
#include<qdebug.h>
ChartView::ChartView(QWidget *parent)
    : QChartView(parent)
{
	m_isPress = false;
	m_ctrlPress = false;
	m_alreadySaveRange = false;
	m_coordItem = nullptr;
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setMouseTracking(true);
    setCursor(Qt::CrossCursor); //设置鼠标指针为手指形
    QPen penY(Qt::darkBlue,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    m_series=new QSplineSeries(this);
    axisX = new QValueAxis(this);
    axisY = new QValueAxis(this);
    Chart=new QChart();
    Chart->setContentsMargins(0, 0, 0, 0);//设置外边界全部为0
    Chart->layout()->setContentsMargins(0,0,0,0);
    Chart->setBackgroundRoundness(0);
    Chart->setMargins(QMargins(0, 0, 0, 0));//设置内边界全部为0
    QFont font = Chart->legend()->font();
    font.setPointSizeF(12);
    Chart->legend()->setFont(font);
    Chart->legend()->setAlignment(Qt::AlignTop);
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
    m_series->attachAxis(axisX);                           //把数据添加到坐标轴上
    m_series->attachAxis(axisY);
    Chart->addSeries(m_series);
    setChart(Chart);

}

ChartView::~ChartView()
{
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
        m_lastPoint = event->pos();
        m_isPress = true;
	}
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
	if (!m_coordItem)
	{
		m_coordItem = new QGraphicsSimpleTextItem(this->chart());
		m_coordItem->setZValue(5);
        m_coordItem->setPos(60, 100);

	}
    m_coordItem->show();
	const QPoint curPos = event->pos();
	QPointF curVal = this->chart()->mapToValue(QPointF(curPos));
	QString coordStr = QString("X = %1, Y = %2").arg(curVal.x()).arg(curVal.y());
	m_coordItem->setText(coordStr);

	if (m_isPress)
	{
		QPoint offset = curPos - m_lastPoint;
		m_lastPoint = curPos;
		if (!m_alreadySaveRange)
		{
			this->saveAxisRange();
			m_alreadySaveRange = true;
		}
		this->chart()->scroll(-offset.x(), offset.y());
	}
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
	m_isPress = false;
	if (event->button() == Qt::RightButton)
	{
		if (m_alreadySaveRange)
		{
			this->chart()->axisX()->setRange(m_xMin, m_xMax);
			this->chart()->axisY()->setRange(m_yMin, m_yMax);
		}
	}
}

void ChartView::wheelEvent(QWheelEvent *event)
{
	const QPoint curPos = event->pos();
	QPointF curVal = this->chart()->mapToValue(QPointF(curPos));

	if (!m_alreadySaveRange)
	{
		this->saveAxisRange();
		m_alreadySaveRange = true;
	}

	const double factor = 1.5;//缩放比例
	if (m_ctrlPress)
	{//Y轴
		QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
		const double yMin = axisY->min();
		const double yMax = axisY->max();
		const double yCentral = curVal.y();

		double bottomOffset;
		double topOffset;
		if (event->delta() > 0)
		{//放大
			bottomOffset = 1.0 / factor * (yCentral - yMin);
			topOffset = 1.0 / factor * (yMax - yCentral);
		}
		else
		{//缩小
			bottomOffset = 1.0 * factor * (yCentral - yMin);
			topOffset = 1.0 * factor * (yMax - yCentral);
		}

		this->chart()->axisY()->setRange(yCentral - bottomOffset, yCentral + topOffset);
	}
	else
	{//X轴
		QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
		const double xMin = axisX->min();
		const double xMax = axisX->max();
		const double xCentral = curVal.x();

		double leftOffset;
		double rightOffset;
		if (event->delta() > 0)
		{//放大
			leftOffset = 1.0 / factor * (xCentral - xMin);
			rightOffset = 1.0 / factor * (xMax - xCentral);
		}
		else
		{//缩小
			leftOffset = 1.0 * factor * (xCentral - xMin);
			rightOffset = 1.0 * factor * (xMax - xCentral);
		}

		this->chart()->axisX()->setRange(xCentral - leftOffset, xCentral + rightOffset);
	}
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Control)
	{
		m_ctrlPress = true;
    }
}

void ChartView::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Control)
	{
		m_ctrlPress = false;
    }
}

void ChartView::leaveEvent(QEvent *)
{
    if(m_coordItem){
         m_coordItem->hide();
    }

}

void ChartView::saveAxisRange()
{
    QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
	m_xMin = axisX->min();
	m_xMax = axisX->max();
    QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
	m_yMin = axisY->min();
    m_yMax = axisY->max();
}

void ChartView::updateseries(QVector<float>&data)
{
    int count=data.size();
    int start = count*(maxsize-1);
    int end=count*maxsize;
    if (buffer.isEmpty()) {
        buffer.reserve(end);
        for (int j = 0; j < end; ++j)
            buffer.append(QPointF(j, 0));
    }

    for (int s = 0; s < start; ++s){
         buffer[s].setY(buffer.at(s + count).y());
    }
    for (int s = start,j=0; s < end&&j<count; s++, j++)
        buffer[s].setY(data.at(j));

    m_series->replace(buffer);


}

void ChartView::updateseries(QVector<QPointF> &data)
{
     if(buffer.isEmpty()){
         buffer.append(data);
     }
     m_series->setUseOpenGL(true);
     m_series->replace(buffer);
}
