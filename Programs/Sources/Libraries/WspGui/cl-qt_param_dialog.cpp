/**
 * @file cl-qt_param_dialog.cpp
 * @author Junichi Nishikata
 */

#include <QtGui/QtGui>

#include "_define_gui.h"

#include <wsp/xerces/cl-xml_handler.h>
#include <wsp/common/cl-std_param_def_list.hpp>

#include "cl-qt_param_dialog.hpp"


#define WSP_QTPARAMDIALOG_TAB_NUM (2)
#define NUM_LINES_TO_GO_NEXT_COLUMN (100) //(10)


#define INIT_WSP_PARAM_DIALOG \
    param_def_list_         (NULL),\
    auto_param_estimate_btn_(new QPushButton("Auto")),\
    start_btn               (new QPushButton("Apply")),\
    cancel_btn              (new QPushButton("Cancel")),\
    load_btn                (new QPushButton("Load XML")),\
    save_btn                (new QPushButton("Save as XML")),\
    btn_hlayout_            (new QHBoxLayout()),\
    extra_hblayout_         (new QHBoxLayout()),\
    tab_glayout_list_       (new QGridLayout[WSP_QTPARAMDIALOG_TAB_NUM]),\
    scroll_area_widget_list_(new QWidget[WSP_QTPARAMDIALOG_TAB_NUM]),\
    tab_scroll_area_list_   (new QScrollArea[WSP_QTPARAMDIALOG_TAB_NUM]),\
    tab_widget_             (new QTabWidget()),\
    main_glayout_           (new QGridLayout()),\
    param_widgets_(NULL),\
    separators_(NULL)




wsp::QtParamDialog::QtParamDialog(
    QWidget *parent, 
    wsp::StdParamDefList *param_def_list, 
    const char *title
)
    : INIT_WSP_PARAM_DIALOG
    , QDialog( parent)
{
    setWindowTitle(title);
    resize(400, 300);

    if(param_def_list==NULL)
    {
        return;
    }
    /* The parental relation of QtParamDialog
     * main_glayout_
     *     tab_widget_
     *         tab_scroll_area_list_[0]
     *             scroll_area_widget_list_[0]
     *                 tab_glayout_list_[0]
     *                     param_vblayout[0]
     *                     param_vblayout[2]
     *                     param_vblayout[3]
     *                     ....
     *         tab_scroll_area_list_[1]
     *             scroll_area_widget_list_[1]
     *                 tab_glayout_list_[1]
     *                     param_vblayout[1]
     *                     param_vblayout[4]
     *                     param_vblayout[5]
     *                     ....
     *     extra_hblayout_
     *     btn_hlayout_
     *         start_btn
     *         load_btn
     *         save_btn
     *         auto_param_estimate_btn_
     *         cancel_btn
     */

    param_def_list_ = param_def_list;

    num_of_params_ = param_def_list_->length();


    // parameters -------------------------------
    int lcdPadding=4;
    int i;
    param_widgets_     = new wsp::QtUnitParamWidget[num_of_params_];
    separators_        = new QFrame             [num_of_params_];

    tab_widget_->addTab(&tab_scroll_area_list_[0], tr("Basic"));
    tab_widget_->addTab(&tab_scroll_area_list_[1], tr("Advanced"));

    const double double_param_mult =  static_cast<double>(pow(10.0, wsp::StdParamDefinition::kDoubleParamPrecision));
    int elem_cnt[WSP_QTPARAMDIALOG_TAB_NUM];
    memset(elem_cnt, 0, WSP_QTPARAMDIALOG_TAB_NUM*sizeof(int));

    for( i=0; i<num_of_params_; ++i )
    {
        param_widgets_[i].setParamDefinition( &(*param_def_list_)[i] );

        {
            int categ_id = (*param_def_list_)[i].categ_id();
            int col = static_cast<int>( elem_cnt[categ_id]/NUM_LINES_TO_GO_NEXT_COLUMN );
            int row = elem_cnt[categ_id] - col*NUM_LINES_TO_GO_NEXT_COLUMN;
            tab_glayout_list_[categ_id].addWidget( &param_widgets_[i], row, col );
            ++(elem_cnt[categ_id]);
        }

        /* add separator */
        {
            separators_[i].setFrameShape(QFrame::HLine);
            separators_[i].setFrameShadow(QFrame::Sunken);
            int categ_id = (*param_def_list_)[i].categ_id();
            int col = static_cast<int>( elem_cnt[categ_id]/NUM_LINES_TO_GO_NEXT_COLUMN );
            int row = elem_cnt[categ_id] - col*NUM_LINES_TO_GO_NEXT_COLUMN;
            tab_glayout_list_[categ_id].addWidget( &separators_[i], row, col );
            ++(elem_cnt[categ_id]);
        }
    }

    for( i=0; i<WSP_QTPARAMDIALOG_TAB_NUM; ++i )
    {
        scroll_area_widget_list_[i].setLayout(&tab_glayout_list_[i]);

        tab_scroll_area_list_[i].setWidget(&scroll_area_widget_list_[i]);
        QScrollBar *scroll_bar = tab_scroll_area_list_[i].verticalScrollBar();
        scroll_bar->setValue(40);
        scroll_bar->setPageStep(20);
        scroll_bar->setValue(int(scroll_bar->value() - scroll_bar->pageStep()/2));
    }
    //scroll_area_->setLayout(param_vblayout);
    //WSP_COMMON_LOG("value=%d, pageStep=%d\n", scrollBar->value(), scrollBar->pageStep());
    //main_glayout_->addWidget(scroll_area_);

    int pos_glayout = 0;
    main_glayout_->addWidget(tab_widget_, pos_glayout++, 0);
    main_glayout_->addLayout(extra_hblayout_, pos_glayout++, 0);
    // ------------------------------------------

    // Footer buttons -----------------------------------
    start_btn->setGeometry(0, 0, 100, 50);
    connect( start_btn, SIGNAL(clicked()), this, SLOT(slot_start()) );

    cancel_btn->setGeometry(0, 50, 200, 50);
    connect( cancel_btn, SIGNAL(clicked()), this, SLOT(close()) );

    load_btn->setGeometry(0, 50, 200, 50);
    connect( load_btn, SIGNAL(clicked()), this, SLOT(slot_loadXml()) );

    save_btn->setGeometry(0, 50, 200, 50);
    connect( save_btn, SIGNAL(clicked()), this, SLOT(slot_saveXml()) );

    auto_param_estimate_btn_->setGeometry(0, 50, 200, 50);
    connect( auto_param_estimate_btn_, SIGNAL(clicked()), this, SLOT(slot_estimateParams()) );

    btn_hlayout_->addWidget(start_btn);
    btn_hlayout_->addWidget(load_btn);
    btn_hlayout_->addWidget(save_btn);
    if( param_def_list_->use_auto_param_estimation() )
    {
        btn_hlayout_->addWidget(auto_param_estimate_btn_);
    }
    btn_hlayout_->addWidget(cancel_btn);
    main_glayout_->addLayout(btn_hlayout_, pos_glayout, 0);
    // ------------------------------------------

    setLayout(main_glayout_);

    this->resize(300, 500);
}

wsp::QtParamDialog::~QtParamDialog()
{
    delete auto_param_estimate_btn_;
    delete cancel_btn;
    delete start_btn;
    delete load_btn;
    delete save_btn;
    delete extra_hblayout_;
    delete btn_hlayout_;
    delete[] param_widgets_;
    delete[] separators_;
    delete[] tab_glayout_list_;
    delete[] scroll_area_widget_list_;
    delete[] tab_scroll_area_list_;
    delete tab_widget_;
    delete main_glayout_;
}

void wsp::QtParamDialog::HideApplyButton()
{
    start_btn->hide();
}

void wsp::QtParamDialog::HideCancelButton()
{
    cancel_btn->hide();
}



//QSize wsp::QtParamDialog::minimumSizeHint() const
//{
//    return QSize(200, 100);
//}
//
//QSize wsp::QtParamDialog::sizeHint() const
//{
//    return QSize(800, 700);
//}

void wsp::QtParamDialog::setToDefault()
{
    if(param_def_list_==NULL){ WSP_COMMON_WARNING_LOG("Param definition list is not set\n"); return; }
    const double double_param_mult =  static_cast<double>(pow(10.0, wsp::StdParamDefinition::kDoubleParamPrecision));

    int i;
    for(i=0; i<num_of_params_; ++i)
    {
        //param_widgets_[i].param_type() = (*param_def_list_)[i].param_type();
        param_widgets_[i].set_param_type( (*param_def_list_)[i].param_type() );

        switch(param_widgets_[i].param_type())
        {
        case WSP_STD_PARAM_INTEGER:
            param_widgets_[i].SetParamSliderValue((*param_def_list_)[i].default_int());
            break;
        case WSP_STD_PARAM_FLOAT:
            param_widgets_[i].SetParamSliderValue((*param_def_list_)[i].default_double()*double_param_mult);
            break;
        case WSP_STD_PARAM_BOOLEAN:
            param_widgets_[i].param_checkbox()->setChecked((*param_def_list_)[i].default_bool());
            break;
        case WSP_STD_PARAM_POINTER_LIST:
            param_widgets_[i].param_combobox()->setCurrentIndex( (*param_def_list_)[i].default_list_index() );
            break;
        case WSP_STD_PARAM_INPUT_FILEPATH:
        case WSP_STD_PARAM_OUTPUT_FILEPATH:
            param_widgets_[i].param_text_edit()->setText((*param_def_list_)[i].file_path());
            break;
        default:
            WSP_COMMON_WARNING_LOG("Unknown parameter type\n");
            continue;
        }

    }

}


wsp::StdParam wsp::QtParamDialog::GetParam(int index){
    wsp::StdParam param;
    WSP_COMMON_LOG(
        "%d: is_double_value=%d, value=%d\n"
        , index, param_widgets_[index].param_text_num()->is_double_value
        , param_widgets_[index].param_slider()->value()
    );
    strcpy(param.name, param_widgets_[index].param_label()->text().toLatin1().data());
    param.id = param_widgets_[index].id();
    switch(param_widgets_[index].param_type())
    {
    case WSP_STD_PARAM_INTEGER:
        param.int_value = param_widgets_[index].param_slider()->value();
        break;
    case WSP_STD_PARAM_FLOAT:
        param.double_value = static_cast<double>(param_widgets_[index].param_slider()->value()) / static_cast<double>(pow(10.0, wsp::StdParamDefinition::kDoubleParamPrecision));
        param.int_value = param_widgets_[index].param_slider()->value();
        break;
    case WSP_STD_PARAM_BOOLEAN:
        param.bool_value = param_widgets_[index].param_checkbox()->isChecked();
        break;
    case WSP_STD_PARAM_POINTER_LIST:
        param.int_value = param_widgets_[index].param_combobox()->currentIndex();
        break;
    case WSP_STD_PARAM_INPUT_FILEPATH:
    case WSP_STD_PARAM_OUTPUT_FILEPATH:
        strcpy(param.file_path, param_widgets_[index].param_text_edit()->text().toLatin1().data());
        break;
    default:
        WSP_COMMON_WARNING_LOG("Unknown paramter\n");
        return param;
    }
    //if(param_widgets_[index].param_text_num()->is_double_value!=0.0){ //! if value is floating point number
    //    param.double_value = static_cast<double>(param_widgets_[index].param_slider()->value()) / static_cast<double>(pow(10.0, wsp::StdParamDefinition::kDoubleParamPrecision));
    //    param.int_value = param_widgets_[index].param_slider()->value();
    //}else{
    //    param.int_value = param_widgets_[index].param_slider()->value();
    //}
    return param;
}

void wsp::QtParamDialog::slot_start(){ accept(); }
void wsp::QtParamDialog::closeEvent( QCloseEvent *e ){ e->accept(); }

void wsp::QtParamDialog::slot_loadXml()
{
    wsp::xerces::XmlHandler xmlHnd;
    QString selectedFilter;
    QString qfilename = QFileDialog::getOpenFileName(NULL, QString("Load XML"),
                            QDir::currentPath(), wsp::formatFilterXml, &selectedFilter);
    if(qfilename.isNull()){ return; }
    //char currentDir[MAX_LENGTH_OF_NAME];
    //wsp::GetDir(currentDir, qfilename.toLatin1().data());
    //if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }
    char paramName[MAX_LENGTH_OF_PATH];

    xmlHnd.LoadParametersFromXml(qfilename.toLatin1().data());

    //cout<<"param_def_list_Size="<<xmlHnd.param_list_->size()<<" "<<xmlHnd.str_param_list_->size()<<endl;

    for(int i=0; i<num_of_params_; i++)
    {
        strcpy(paramName, param_widgets_[i].param_label()->text().toLatin1().data());
        //WSP_COMMON_LOG("%s: has=%d, value=%d\n", paramName,
        //                                xmlHnd.HasParameter(paramName),
        //                                (*xmlHnd.param_list_)[paramName]);
        if(xmlHnd.HasParameter(paramName))
        {
            switch(param_widgets_[i].param_type()){
            case WSP_STD_PARAM_INTEGER:
                param_widgets_[i].SetParamSliderValue(xmlHnd.GetIntParameter(paramName));
                break;
            case WSP_STD_PARAM_FLOAT:
                param_widgets_[i].SetParamSliderValue(xmlHnd.GetIntParameter(paramName));
                break;
            case WSP_STD_PARAM_BOOLEAN:
                param_widgets_[i].param_checkbox()->setChecked(static_cast<bool>(xmlHnd.GetIntParameter(paramName)));
                break;
            case WSP_STD_PARAM_POINTER_LIST:
                param_widgets_[i].param_combobox()->setCurrentIndex(xmlHnd.GetIntParameter(paramName));
                break;
            case WSP_STD_PARAM_INPUT_FILEPATH:
            case WSP_STD_PARAM_OUTPUT_FILEPATH:
                {
                    QString tmp_path(xmlHnd.GetStringParameter(paramName).c_str());
                    param_widgets_[i].param_text_edit()->setText(tmp_path);
                }
                break;
            default:
                WSP_COMMON_WARNING_LOG("Unknown paramter\n");
                continue;
            }
        }
    }
}

void wsp::QtParamDialog::slot_saveXml()
{
    wsp::xerces::XmlHandler xmlHnd;
    QString selectedFilter;
    QString qfilename = QFileDialog::getSaveFileName(NULL, QString("Save As"),
                            QDir::currentPath(), wsp::formatFilterXml, &selectedFilter);
    char currentDir[MAX_LENGTH_OF_NAME];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }

    for(int i=0; i<num_of_params_; i++)
    {
        std::string param_name = std::string(param_widgets_[i].param_label()->text().toLatin1().data());
        switch(param_widgets_[i].param_type())
        {
        case WSP_STD_PARAM_INTEGER:
        case WSP_STD_PARAM_FLOAT:
            xmlHnd.AddIntParameter(param_name, param_widgets_[i].param_slider()->value());
            break;
        case WSP_STD_PARAM_BOOLEAN:
            xmlHnd.AddIntParameter(param_name, static_cast<int>(param_widgets_[i].param_checkbox()->isChecked()));
            break;
        case WSP_STD_PARAM_POINTER_LIST:
            xmlHnd.AddIntParameter(param_name, static_cast<int>(param_widgets_[i].param_combobox()->currentIndex()));
            break;
        case WSP_STD_PARAM_INPUT_FILEPATH:
            xmlHnd.AddStringParameter(param_name, std::string(param_widgets_[i].param_text_edit()->text().toLatin1().data()));
            break;
        default:
            WSP_COMMON_WARNING_LOG("Unknown paramter\n");
            continue;
        }
    }

    xmlHnd.SaveParametersAsXml(qfilename.toLatin1().data());
}

void wsp::QtParamDialog::SetParamSliderValue( int index, int value )
{
    param_widgets_[index].SetParamSliderValue( value );
}
