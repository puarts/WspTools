/**
 * @file _cl-std_param.h
 * 
 */

#ifndef __WSP_CORE_CLASS_WSP_STD_PARAM_H__
#define __WSP_CORE_CLASS_WSP_STD_PARAM_H__

#include <iomanip>

#ifndef __WSP_COMMONCORE_STRUCT_STDPARAM_H__
#include "st-stdparam.h"
#endif

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif


#include "tpl_cl-vector.h"

namespace wsp{




    // Param definition ------------------------------------------------------


    //******************************************************************************
    //* CLASS DECLARATION (wsp::StdParamDefinition)
    //! \ingroup WspCommonUtil
    //! \brief Standard abstract parameter
    class StdParamDefinition
    {
    public:
        static const int kDoubleParamPrecision=4; /* number of digits for double parameter display */

    public:
        // Constructor ----------------------------------------------
        inline StdParamDefinition();
        inline explicit StdParamDefinition(
            int         id,
            const char* in_name, 
            int         in_min, 
            int         in_max, 
            int         in_default, 
            const char* in_description=NULL
            ,int categ_id=0
        );
        inline explicit StdParamDefinition(
            int         id,
            const char* in_name, 
            double      in_min, 
            double      in_max, 
            double      in_default, 
            const char* in_description=NULL
            ,int categ_id=0
        );
        inline explicit StdParamDefinition(
            int         id,
            const char* in_name, 
            bool        in_default, 
            const char* in_description=NULL
            ,int categ_id=0
        );
        inline explicit StdParamDefinition(
            int         id,
            const char* in_name, 
            const char* file_path, 
            const char* in_description=NULL
            ,int categ_id=0
        );
        inline explicit StdParamDefinition(
            int         id,
            const char* in_name, 
            const wsp::Vector<void*> **ptr_list,
            int         default_list_index,
            const char* in_description=NULL
            ,int categ_id=0
        );
        // this constructor is for 0 conversion
        inline explicit StdParamDefinition(int value);


        // Getter ------------------------------------------------
        const wsp::StdParamType param_type         () const{ return param_type_; }
        const char*             name               () const{ return name_; }
        const char*             description        () const{ return description_; }
        int                     max_int            () const{ return max_int_; }
        int                     min_int            () const{ return min_int_; }
        int                     default_int        () const{ return default_int_; }
        double                  max_double         () const{ return max_double_; }
        double                  min_double         () const{ return min_double_; }
        double                  default_double     () const{ return default_double_; }
        bool                    default_bool       () const{ return default_bool_; }
        bool                    IsDoubleParam      () const{ return param_type_==WSP_STD_PARAM_FLOAT; }
        const char*             file_path          () const{ return file_path_; }
        const wsp::Vector<void*>* ptr_list         () const{ return *ptr_list_; }
        int                     default_list_index () const{ return default_list_index_; }
        int                     param_id           () const{ return param_id_; }
        int                     categ_id           () const{ return categ_id_; }

        // Setter ------------------------------------------------
        void set_param_type        (wsp::StdParamType param_type){ param_type_ = param_type; }
        void set_max_int           (int value   ){ max_int_            = value; }
        void set_min_int           (int value   ){ min_int_            = value; }
        void set_default_int       (int value   ){ default_int_        = value; }
        void set_max_double        (double value){ max_double_         = value; }
        void set_min_double        (double value){ min_double_         = value; }
        void set_default_double    (double value){ default_double_     = value; }
        void set_default_bool      (bool value  ){ default_bool_       = value; }
        void set_default_list_index(int value   ){ default_list_index_ = value; }
        void set_file_path         (const char *file_path){
            if(sizeof(file_path)>=MAX_LENGTH_OF_PATH){
                WSP_COMMON_ERROR_LOG("length of name exceed max length of path\n"); return;
            }
            sprintf(file_path_, file_path);
        }
        void SetName(const char *in_name){
            if(sizeof(in_name)>=MAX_LENGTH_OF_NAME){
                WSP_COMMON_ERROR_LOG("length of name exceed max length of name\n"); return;
            }
            sprintf(name_, in_name); 
        }
        void SetDiscription(const char *in_description){
            if(sizeof(in_description)>=MAX_LENGTH_OF_DISCRIPTION){
                WSP_COMMON_ERROR_LOG("length of description exceed max length of description\n"); return;
            }
            sprintf(description_, in_description); 
        }
        void SetPointerList(
            int         id,
            const char* in_name, 
            const wsp::Vector<void*> **ptr_list,
            int         default_list_index,
            const char* in_description=NULL,
            int categ_id=0
        )
        {
            sprintf(name_, in_name);
            ptr_list_ = ptr_list;
            default_list_index_ = default_list_index;

            if(in_description!=NULL){ SetDiscription(in_description); }
            param_type_ = WSP_STD_PARAM_POINTER_LIST;
            param_id_ = id;
            categ_id_ = categ_id;
            WSP_COMMON_DEBUG_LOG("parameter \"%s\" was assumed as %s\n", in_name, WSP_GetStdParamTypeAsString(param_type_));
        }


    private:
        inline void Init();

    private:
        char name_[MAX_LENGTH_OF_NAME];
        char description_[MAX_LENGTH_OF_DISCRIPTION];
        wsp::StdParamType param_type_;
        int param_id_;
        int categ_id_;

        // data definition of parameter
        union{
            char file_path_[MAX_LENGTH_OF_PATH];
            struct{
                const wsp::Vector<void*> **ptr_list_;
                int default_list_index_;
            };
            struct{
                union{
                    double min_double_;
                    int min_int_;
                };
                union{
                    double default_double_;
                    int default_int_;
                };
                union{
                    double max_double_;
                    int max_int_;
                };
            };
            bool default_bool_;
        };
        
    };

    inline double ConvertIntegerValueToDoubleParam( int value )
    {
        return value / static_cast<double>(pow(10.0, wsp::StdParamDefinition::kDoubleParamPrecision));
    }




    inline void SetDefaultParamFromDefinition( wsp::StdParam *param, wsp::StdParamDefinition *def )
    {
        param->id = def->param_id();
        switch( def->param_type() )
        {
        case WSP_STD_PARAM_INTEGER         : 
            param->int_value = def->default_int();
            break;
        case WSP_STD_PARAM_FLOAT           :
            param->double_value = def->default_double();
            break;
        case WSP_STD_PARAM_BOOLEAN         :
            param->bool_value = def->default_bool();
            break;
        case WSP_STD_PARAM_POINTER_LIST:
            param->int_value = def->default_list_index();
            break;
        case WSP_STD_PARAM_INPUT_FILEPATH  :
        case WSP_STD_PARAM_OUTPUT_FILEPATH :
            strcpy( param->file_path, def->file_path() );
            break;
        case WSP_STD_PARAM_UNDEFINED       :
        default:
            break;
        }
    }
    inline void PrintStdParam( const wsp::StdParam *param, const wsp::StdParamDefinition *param_def )
    {
        WSP_COMMON_LOG( "%d %s: ", param->id, param_def->name() );
        switch( param_def->param_type() )
        {
        case WSP_STD_PARAM_INTEGER        :
            WSP_COMMON_LOG( "%d\n", param->int_value ); 
            break;
        case WSP_STD_PARAM_FLOAT          :
            WSP_COMMON_LOG( "%f\n", param->double_value ); 
            break;
        case WSP_STD_PARAM_BOOLEAN        :
            WSP_COMMON_LOG( "%s\n", param->bool_value? "true" : "false" ); 
            break;
        case WSP_STD_PARAM_POINTER_LIST:
            WSP_COMMON_LOG( "%d\n", param->int_value );
            break;
        case WSP_STD_PARAM_INPUT_FILEPATH :
        case WSP_STD_PARAM_OUTPUT_FILEPATH:
            WSP_COMMON_LOG( "%s\n", param->file_path ); 
            break;
        case WSP_STD_PARAM_UNDEFINED      :
            WSP_COMMON_LOG( "undefined type\n" ); 
            break;
        }
    }
}

inline std::ostream& operator<<(std::ostream& lhs, const wsp::StdParamDefinition &rhs)
{
    lhs<<std::setw(PRINT_COLW)<<std::setprecision(PRINT_PREC);
    //lhs<<scientific;
    lhs<<PRINT_TYPE;
    lhs<<rhs.min_int()<<" "<<rhs.max_int()<<" "<<rhs.default_int()<<std::endl;
    lhs<<rhs.min_double()<<" "<<rhs.max_double()<<" "<<rhs.default_double()<<std::endl;
    lhs<<(rhs.default_bool()?"true":"false")<<std::endl;
    lhs<<rhs.default_list_index()<<std::endl;
    lhs<<rhs.file_path()<<std::endl;
    return lhs;
}


inline std::ostream& operator<<(std::ostream& lhs, const wsp::StdParam &rhs)
{
    lhs<<std::setw(PRINT_COLW)<<std::setprecision(PRINT_PREC);
    //lhs<<scientific;
    lhs<<PRINT_TYPE;
    lhs<<rhs.int_value<<std::endl;
    lhs<<rhs.double_value;
    lhs<<(rhs.bool_value?"true":"false")<<std::endl;
    lhs<<rhs.int_value<<std::endl;
    lhs<<rhs.file_path<<std::endl;
    return lhs;
}


inline void wsp::StdParamDefinition::Init()
{
    param_type_ = WSP_STD_PARAM_UNDEFINED;
    sprintf(name_, "");
    sprintf(description_, "");
    sprintf(file_path_, "");
    memset(file_path_, 0, sizeof(file_path_));
}


#define INIT_WSP_WSP_STD_PARAM_DEFINITION \
    param_type_(WSP_STD_PARAM_UNDEFINED),\
    max_double_(0.0),\
    min_double_(0.0),\
    default_double_(0.0),\
    param_id_(0),\
    categ_id_(0)

// Constructor ----------------------------------------------
inline wsp::StdParamDefinition::StdParamDefinition()
    :INIT_WSP_WSP_STD_PARAM_DEFINITION
{
    Init();
    sprintf(name_, "defaultParam");
}

inline wsp::StdParamDefinition::StdParamDefinition(
    int id,
    const char *in_name, 
    int in_min, int in_max, int in_default, 
    const char *in_description
    ,int categ_id
)
    :INIT_WSP_WSP_STD_PARAM_DEFINITION
{
    Init();
    sprintf(name_, in_name);
    if(in_description!=NULL){ SetDiscription(in_description); }
    min_int_ = in_min; 
    max_int_ = in_max; 
    default_int_ = in_default;
    param_type_ = WSP_STD_PARAM_INTEGER;
    param_id_ = id;
    categ_id_ = categ_id;
    WSP_COMMON_DEBUG_LOG("%s was assumed as %s\n", in_name, WSP_GetStdParamTypeAsString(param_type_));
}

inline wsp::StdParamDefinition::StdParamDefinition(
    int id,
    const char *in_name, 
    double in_min, double in_max, double in_default, 
    const char *in_description
    ,int categ_id
)
    :INIT_WSP_WSP_STD_PARAM_DEFINITION
{
    Init();
    sprintf(name_, in_name);
    if(in_description!=NULL){ SetDiscription(in_description); }
    min_double_ = in_min; 
    max_double_ = in_max; 
    default_double_ = in_default;
    param_type_ = WSP_STD_PARAM_FLOAT;
    param_id_ = id;
    categ_id_ = categ_id;
    WSP_COMMON_DEBUG_LOG("%s was assumed as %s\n", in_name, WSP_GetStdParamTypeAsString(param_type_));
}

inline wsp::StdParamDefinition::StdParamDefinition(
    int id,
    const char *in_name, 
    bool in_default, 
    const char *in_description
    ,int categ_id
)
    :INIT_WSP_WSP_STD_PARAM_DEFINITION
{
    Init();
    sprintf(name_, in_name);
    if(in_description!=NULL){ SetDiscription(in_description); }
    default_bool_ = in_default;
    param_type_ = WSP_STD_PARAM_BOOLEAN;
    param_id_ = id;
    categ_id_ = categ_id;
    WSP_COMMON_DEBUG_LOG("%s was assumed as %s\n", in_name, WSP_GetStdParamTypeAsString(param_type_));

}

inline wsp::StdParamDefinition::StdParamDefinition(
    int id,
    const char *in_name, 
    const char *file_path, const char *in_description
    ,int categ_id
)
    :INIT_WSP_WSP_STD_PARAM_DEFINITION
{
    Init();
    sprintf(name_, in_name);
    set_file_path(file_path);
    if(in_description!=NULL){ SetDiscription(in_description); }
    param_type_ = WSP_STD_PARAM_INPUT_FILEPATH;
    param_id_ = id;
    categ_id_ = categ_id;
    WSP_COMMON_DEBUG_LOG("%s was assumed as %s\n", in_name, WSP_GetStdParamTypeAsString(param_type_));
}

inline wsp::StdParamDefinition::StdParamDefinition(
    int         id,
    const char* in_name, 
    const wsp::Vector<void*> **ptr_list,
    int         default_list_index,
    const char* in_description
    ,int categ_id
)
    :INIT_WSP_WSP_STD_PARAM_DEFINITION
{
    Init();
    sprintf(name_, in_name);
    ptr_list_ = ptr_list;
    if(in_description!=NULL){ SetDiscription(in_description); }
    param_type_ = WSP_STD_PARAM_POINTER_LIST;
    param_id_ = id;
    categ_id_ = categ_id;
    WSP_COMMON_DEBUG_LOG("%s was assumed as %s\n", in_name, WSP_GetStdParamTypeAsString(param_type_));
}

// this constructor is for 0 conversion
inline wsp::StdParamDefinition::StdParamDefinition(int value)
    :INIT_WSP_WSP_STD_PARAM_DEFINITION
{
    Init();
}



#endif
