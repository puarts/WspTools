/**
 * @file cl-qt_transform_param_widget.hpp
 * @author Junichi Nishikata
 */

#ifndef WSP_GUI_CLASS_QT_TRANSFORM_PARAM_WIDGET_HPP__
#define WSP_GUI_CLASS_QT_TRANSFORM_PARAM_WIDGET_HPP__

#include <QtGui/QtGui>

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#ifndef __WSP_COMMONCORE_CTYPE_H__
#include <wsp/common/_ctypes.h>
#endif

#ifndef __WSP_COMMONCORE_FUNCTION_IO_H__
#include <wsp/common/fn-io.h>
#endif

namespace wsp{

	typedef struct TransformParam{
		QString name;
		u32     dimension;
		f32     default_values[3];
        f32     min_value;
        f32     max_value;
        f32     step;
        f32*    dest_values[3];
	} TransformParam;


	class WSP_DLL_EXPORT QtTransformParamWidget
		: public QWidget
	{
        Q_OBJECT
	public:
		QtTransformParamWidget( 
            const TransformParam &param
        ) : key_selected_act_( new QAction(QString("key Selected"), &pop_menu_) )
		{
			QDoubleSpinBox* ptr     = spinbox_;
			QDoubleSpinBox* end_ptr = ptr + param.dimension;
			//f32* val_ptr = param.default_values;
			for( int i=0; ptr!=end_ptr; ++ptr, ++i )
			{
				ptr->setRange( -3600, 3600 );
				ptr->setValue( *param.dest_values[i] );
                ptr->setSingleStep( (double)param.step );
                ptr->setMinimum( param.min_value );
                ptr->setMaximum( param.max_value );
                main_layout_.addWidget( ptr );
			}
			this->setLayout( &main_layout_ );

            /* --------------------------------- 
                    Set up context menu 
               ---------------------------------*/
            {
                connect( key_selected_act_, SIGNAL(triggered()), this, SLOT(slot_keySelected()));
                pop_menu_.addAction(key_selected_act_);
                pop_menu_.addSeparator();
                this->setContextMenuPolicy(Qt::CustomContextMenu);
                connect( 
                    this, SIGNAL(customContextMenuRequested(const QPoint&)),
                    this, SLOT(slot_showContextMenu(const QPoint&))
                );
            }
		}
		~QtTransformParamWidget()
        {
            delete key_selected_act_;
        }

        void setSpinboxValues( f32 value0, f32 value1, f32 value2 )
        {
            spinbox_[0].setValue( value0 );
            spinbox_[1].setValue( value1 );
            spinbox_[2].setValue( value2 );
        }

		void set_dest_values( f32* value_0, f32* value_1, f32* value_2)
        { 
            dest_values_[0] = value_0;
            dest_values_[1] = value_1;
            dest_values_[2] = value_2;

            connect(
                    &spinbox_[0], SIGNAL(valueChanged(double)),
                    this,         SLOT(slot_updateDestValue0(double))
                );
            connect(
                    &spinbox_[1], SIGNAL(valueChanged(double)),
                    this,         SLOT(slot_updateDestValue1(double))
                );
            connect(
                    &spinbox_[2], SIGNAL(valueChanged(double)),
                    this,         SLOT(slot_updateDestValue2(double))
                );

        }

    signals:
        void signal_valueUpdated();
        void signal_keySelected();

    private slots:
        void slot_updateDestValue0( double value )
        {
            //WSP_COMMON_DEBUG_LOG("\n");
            *dest_values_[0] = (f32)value;
            emit signal_valueUpdated();
        }
        void slot_updateDestValue1( double value )
        {
            //WSP_COMMON_DEBUG_LOG("\n");
            *dest_values_[1] = (f32)value;
            emit signal_valueUpdated();
        }
        void slot_updateDestValue2( double value )
        {
            //WSP_COMMON_DEBUG_LOG("\n");
            *dest_values_[2] = (f32)value;
            emit signal_valueUpdated();
        }
        void slot_keySelected()
        {
            WSP_COMMON_DEBUG_LOG( "\n" );
            emit signal_keySelected();
        }
        void slot_showContextMenu(const QPoint& pos)
        {
            QPoint global_pos = this->mapToGlobal(pos);
            QAction* selected_item = pop_menu_.exec(global_pos);
        }


	private:
		/* data */
        f32* dest_values_[3];

		/* GUI */
		QDoubleSpinBox spinbox_[3];
		QHBoxLayout    main_layout_;

        QMenu          pop_menu_;
        QAction*       key_selected_act_;
	};
}


#endif