#define USE_FLTK13

#pragma warning(disable : 4819) // disables warning messages

#ifdef USE_FLTK13

#ifdef _DEBUG
    #pragma comment(lib, "fltkd.lib") 
    #pragma comment(lib, "fltkformsd.lib") 
    #pragma comment(lib, "fltkld.lib") 
    #pragma comment(lib, "fltkimagesd.lib") 
    #pragma comment(lib, "fltkjpegd.lib") 
    #pragma comment(lib, "fltkpngd.lib") 
    #pragma comment(lib, "fltkzlibd.lib") 
#else
    #pragma comment(lib, "fltk.lib") 
    #pragma comment(lib, "fltkforms.lib") 
    #pragma comment(lib, "fltkl.lib") 
    #pragma comment(lib, "fltkimages.lib") 
    #pragma comment(lib, "fltkjpeg.lib") 
    #pragma comment(lib, "fltkpng.lib") 
    #pragma comment(lib, "fltkzlib.lib") 
#endif

#endif