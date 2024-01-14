/**
 * @file cl-string.cpp
 * 
 */

#include "cl-string.h"

wsp::String wsp::String::Split(const char deliminator, int fetch_index) const
{
    if(length()==0){ return wsp::String(""); }
    wsp::String elem;
    elem.Clear();
    int i, i_elem, index;
    int len = strlen(data_);
    for(i=0, i_elem=0, index=0; i<len; i++){
        if(data_[i]==deliminator){
            elem[i_elem] = '\0';
            if(index == fetch_index){ return elem; }
            elem.Clear();
            i_elem=0;
            index++;
            continue;
        }
        elem[i_elem]=data_[i];
        i_elem++;
    }
    if(data_[i-1]!=deliminator){
        elem[i_elem] = '\0';
        if(index == fetch_index){ return elem; }
    }
    return wsp::String("");
}

wsp::Vector<wsp::String> wsp::String::Split(const char deliminator) const
{
    wsp::Vector<wsp::String> list;
    if(length()==0){ return list; }
    wsp::String elem;
    elem.Clear();
    int i, i_elem;
    for(i=0, i_elem=0; i<length(); i++){
        if(data_[i]==deliminator){
            elem[i_elem] = '\0';
            list.Append(elem);
            elem.Clear();
            i_elem=0;
            continue;
        }
        elem[i_elem]=data_[i];
        i_elem++;
    }
    if(data_[i-1]!=deliminator){
        elem[i_elem] = '\0';
        list.Append(elem);
    }
    return list;
}

