//! WspQtGraphViewer.cpp
// 

#include "WspQtGraphViewer.h"

#ifdef USE_QT

// Constructor, Destructor --------------------------------------------
#define INIT_WspQtGraphViewer width(300),height(300),y_min(0.0), y_max(0.0), x_min(0.0), x_max(0.0), padding(40)

void WspQtGraphViewer::Init(){
    QPalette btn_plt;
    btn_plt.setBrush(QPalette::ButtonText, QBrush(QColor(0,0,0)));

    renderArea = WSP_NEW QLabel();
    renderArea->resize(width, height);
    infoArea = WSP_NEW QLabel();
    infoArea->resize(width, 100);

    closeButton = WSP_NEW QPushButton(tr("&Close"));
    closeButton->setPalette(btn_plt);

    resultImage = QPixmap(width, height);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    setWindowTitle(tr("Wsp Graph Viewer"));
    resize(width, height);
    setBackgroundRole(QPalette::Base);
    setPalette(QPalette(QColor(200,200,200)));
    setAutoFillBackground(true);

    QGridLayout *mainLayout = WSP_NEW QGridLayout;
    mainLayout->addWidget(renderArea);
    mainLayout->addWidget(infoArea);
    mainLayout->addWidget(closeButton);
    setLayout(mainLayout);
}
WspQtGraphViewer::WspQtGraphViewer()
    :INIT_WspQtGraphViewer
{ Init(); }
WspQtGraphViewer::~WspQtGraphViewer()
{}

// -------------------------------------------- end Constructor, Destructor


void WspQtGraphViewer::ShowGraph(const QString& msg){
    m_msg=msg;
    resize(width, height);
    this->exec();
}

void WspQtGraphViewer::paintEvent(QPaintEvent*){
    if(length_==0 || data_==NULL){ return; }

    wsp::Vector<double> min, max;
    double min_all, max_all, start, end;
    u32 x1, y1, x2, y2, length, itr_fn, itr_i, w, h, rad, linew;
    QColor col;
    QPainter painter(&resultImage);
    QPen pen;
    QString sRow, sCol, sNum, sSt, sEnd, sStep, sMin, sMax, msg;
    bool line, dot;

    start = data_[0].getStart();
    end = data_[0].getEnd();

    sRow.setNum(length_);
    sCol.setNum(end-start+1);
    sSt.setNum(start);
    sEnd.setNum(end);
    sStep.setNum(data_[0].getStep());


    min.SetLength(length_);
    max.SetLength(length_);

    painter.setPen(QPen(Qt::black, 0));
    w = width-2*padding;
    h = height-2*padding;

    // finding max and min
    if(y_max==0.0 && y_min==0.0)
    {
        min_all = min[0] = data_[0].GetMin();
        max_all = max[0] = data_[0].GetMax();
        for(itr_fn=1; itr_fn<length_; itr_fn++){
            min[itr_fn] = data_[itr_fn].GetMin();
            max[itr_fn] = data_[itr_fn].GetMax();
            if(min_all > min[itr_fn]){
                min_all = min[itr_fn];
            }
            if(max_all < max[itr_fn]){
                max_all = max[itr_fn];
            }
        }
    }
    else
    {
        min_all = y_min;
        max_all = y_max;
    }

    // painting graph
    for(itr_fn=0; itr_fn<length_; itr_fn++){
        //value = 255 - itr_fn*(255/(length_+1));
        //col.setHsv(value, 255, 255);
        line = data_[itr_fn].drawLine;
        dot = data_[itr_fn].drawDot;
        rad = data_[itr_fn].dot_size;
        linew = data_[itr_fn].line_width;

        pen.setColor( QColor( data_[itr_fn].m_color) );
        pen.setWidth(linew);
        painter.setPen(pen);
        length = data_[itr_fn].length();
        x1 = (u32)padding;
        y1 = (u32)(h*(data_[itr_fn][0] - min_all)/(max_all - min_all) + padding);
        if(dot!=false){
            painter.drawEllipse(x1-rad, height-y1-rad, 2*rad, 2*rad);
        }
        for(itr_i=1; itr_i<length; itr_i++){
            x2 = (u32)(itr_i * w/(double)length + padding);
            y2 = (u32)(h * (data_[itr_fn][itr_i]-min_all)/(max_all - min_all) + padding);
            if(line!=false){
                painter.drawLine(x1, height-y1, x2, height-y2);
            }
            if(dot!=false){
                painter.drawEllipse(x2-rad, height-y2-rad, 2*rad, 2*rad);
            }
            x1 = x2;
            y1 = y2;
        }
    }

    // drowing line for x axis
    x1=padding;
    y1=h*(0.0-min_all)/(max_all-min_all) + padding;
    x2=w + padding;
    y2=h*(0.0-min_all)/(max_all-min_all) + padding;
    pen.setColor(QColor(100,100,205));
    painter.setPen(pen);
    painter.drawLine(x1, height-y1, x2, height-y2);
    painter.drawText(x1, height-y1+10, sSt);
    painter.drawText(x2, height-y2+10, sEnd);

    // drowing line for y axis
    y1=padding;
    x1=w*(0.0-start)/(end-start) + padding;
    y2=h + padding;
    x2=w*(0.0-start)/(end-start) + padding;
    sMax.setNum(max_all);
    sMin.setNum(min_all);
    painter.drawLine(x1, height-y1, x2, height-y2);
    painter.drawText(x2, height-y2, sMax);
    painter.drawText(x1, height-y1, sMin);


    // displays max and min for each function
    msg =m_msg+"\n";
    msg+=tr("function=")+sRow;
    msg+=tr(", sample=")+sCol;
    msg+=tr(", start=")+sSt;
    msg+=tr(", end=")+sEnd;
    msg+=tr(", step=")+sStep+tr("\n");
    for(itr_fn=0; itr_fn<length_; itr_fn++){
        sNum.setNum(itr_fn);
        sMin.setNum(data_[itr_fn].GetMin());
        sMax.setNum(data_[itr_fn].GetMax());
        msg+=sNum;
        msg+=tr(": max=")+sMax;
        msg+=tr(",   min=")+sMin+tr("\n");
    }
    painter.end();
    renderArea->setPixmap(resultImage);
    infoArea->setText(msg);
}

//
//void WspQtGraphViewer::showData(u32 index){
//    QString sLength, msg;
//    QString *sData;
//    u32 length;
//    length = data_[index].length();
//    sLength.setNum(length);
//    msg = QString("length:") + sLength;
//
//    sData = new QString[length + RESERVE_MEM];
//    for(u32 itr_i=0; itr_i<length; itr_i++){
//        sData[itr_i].setNum(data_[index][itr_i]);
//        msg += QString("\n")+sData[itr_i];
//    }
//
//    QMessageBox::information(NULL, QString("debug"),msg);
//    delete[] sData;
//}



#endif