//! WspWspHttpURLContent.h
// @author Junichi Nishikata


// Example showing the use --------------------------
//int main(int argc, char *argv[])
//{
//  string url;
//  int number = 1;
//  if(argc == 2){
//        url = argv[1];
//        printf("input show line : ");
//        scanf("%d",&number);
//  } else {
//        cerr << "Usage: " << argv[0] << " url" << endl;
//        return(1);
//  }
//  WspHttpURLContent huc(url);
//  huc.getContent(number);
//
//  return(0);
//}
//---------------------------------------------------


#ifndef _WspHttpURLContent_H_
#define _WspHttpURLContent_H_

#include <string>
#include <cstdio>
#include <errno.h>
#include <wsp/web/_WspDefine_Web.h>

#define C_BUFFER_LENGTH (1024)
//extern int errno;

using namespace std;

class WspHttpURLContent
{
private:
    string url_;

public:
    WSP_DLL_EXPORT WspHttpURLContent();
    WSP_DLL_EXPORT WspHttpURLContent(string url);
    WSP_DLL_EXPORT ~WspHttpURLContent();

    // Setter
    inline void SetURL(const string url){ url_=url; }

    // Getter
    inline string url(){ return url_; }
    WSP_DLL_EXPORT string::size_type ParseURL(string& out_host, string& out_service, string& out_path);
    WSP_DLL_EXPORT void GetContent(string &out_content, int line);

};

#endif