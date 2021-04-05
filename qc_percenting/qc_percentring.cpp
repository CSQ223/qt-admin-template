#include "qc_percentring.h"

QC_PercentRing::QC_PercentRing(QWidget *parent) : QWidget(parent)
{
    value = 0;

    textColor = QColor(0, 0, 0);
    outerCircleColor = QColor(255, 80, 80);
    midCircleColor = QColor(255, 255, 255);
    innerCircleColor = QColor(141, 212, 255);
    centerCircleColor = QColor(255, 255, 255);
    pieColorStart = QColor(70, 172, 255);
}

void QC_PercentRing::setTextColor(QColor color)
{
    textColor = color;

    update();
}

void QC_PercentRing::setOutCircleColor(QColor color)
{
    outerCircleColor = color;

    update();
}

void QC_PercentRing::setMidCircleColor(QColor color)
{
    midCircleColor = color;

    update();
}

void QC_PercentRing::setInnerCircleColor(QColor color)
{
    innerCircleColor = color;

    update();
}

void QC_PercentRing::setCenterCircleColor(QColor color)
{
    centerCircleColor = color;

    update();
}

void QC_PercentRing::setPieColorStart(QColor color)
{
    pieColorStart = color;

    update();
}

void QC_PercentRing::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);

    //绘制外圆
    drawOuterCircle(&painter);

    drawMiddleCircle(&painter);
    //绘制内圆
    drawInterCircle(&painter);

    drawScale(&painter);

    drawCenterCircle(&painter);

    drawText(&painter);
}

void QC_PercentRing::drawOuterCircle(QPainter *painter)
{
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(outerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void QC_PercentRing::drawMiddleCircle(QPainter *painter)
{
    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(midCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void QC_PercentRing::drawInterCircle(QPainter *painter)
{
    int radius = 85;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(innerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void QC_PercentRing::drawCenterCircle(QPainter *painter)
{
    int radius = 50;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(centerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void QC_PercentRing::drawScale(QPainter *painter)
{
    int radius = 85;
    painter->save();
    painter->setPen(Qt::NoPen);

    QRectF rect(-radius, -radius, radius * 2, radius * 2);

    //if (pieStyle == PieStyle_Three) {
        //计算总范围角度,根据占比例自动计算三色圆环范围角度
        //可以更改比例
    double angleAll = 360.0;
    double angleStart = angleAll * value / 100;
    //double angleMid = angleAll * 0.15;
    //double angleEnd = angleAll * 0.15;

    //增加偏移量使得看起来没有脱节
//    int offset = 3;

    //绘制开始饼圆
    painter->setBrush(pieColorStart);
    painter->drawPie(rect,  - 90.0 * 16, -angleStart * 16);

        //绘制中间饼圆
        //painter->setBrush(pieColorMid);
        //painter->drawPie(rect, (270 - startAngle - angleStart - angleMid) * 16 + offset, angleMid * 16);

        //绘制结束饼圆
        //painter->setBrush(pieColorEnd);
        //painter->drawPie(rect, (270 - startAngle - angleStart - angleMid - angleEnd) * 16 + offset * 2, angleEnd * 16);
    painter->restore();
}

void QC_PercentRing::drawText(QPainter *painter)
{
    int radius = 100;
    painter->save();
    painter->setPen(textColor);

    QFont font;
    font.setPixelSize(18);
    painter->setFont(font);

    QRectF textRect(-radius, -radius, radius * 2, radius * 2);
    QString strValue = QString("%1%").arg(value);
    painter->drawText(textRect, Qt::AlignCenter, strValue);

    painter->restore();
}

void QC_PercentRing::setValue(int data)
{
    this->value = data;

    update();
}
