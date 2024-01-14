/**
 * @file WspXmlHandler.cpp
 * 
 */

#include <wsp/xerces/cl-xml_handler.h>

#include <wsp/xerces/fn-xml_utilities.h>
#include <wsp/common/fn-io.h>
#include <wsp/common/_define_commoncore.h>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_USE

#include <iostream>
#include <stdio.h>
#include <ctype.h>

using namespace std;

// Constructor, Destructor -----------------------------------
wsp::xerces::XmlHandler::XmlHandler()
{
}

wsp::xerces::XmlHandler::~XmlHandler()
{
}

wsp::xerces::XmlHandler::XmlHandler(wsp::xerces::XmlHandler &rhs)
{
}

// Getter ---------------------------------------------------
bool wsp::xerces::XmlHandler::HasParameter(const char *param_name) const
{
    //map<string, int>::iterator iter_param = param_list_.find(string(param_name));
    //map<string, string>::iterator iter_io = str_param_list_.find(string(param_name));

    int cnt = 0;
    cnt += param_list_.count(std::string(param_name));
    cnt += str_param_list_.count(std::string(param_name));
    cnt += float_param_list_.count(std::string(param_name));
    cnt += bool_param_list_.count(std::string(param_name));

    if(cnt!=1){
        return false;
    }
    //if(iter_param==param_list_.end() && iter_io==str_param_list_.end()){
    //    return false;
    //}
    return true;
}

bool wsp::xerces::XmlHandler::GetBoolParameter(const std::string &param_name) const
{
    return bool_param_list_.at(param_name);
}

int wsp::xerces::XmlHandler::GetIntParameter(const std::string &param_name) const
{
    return param_list_.at(param_name);
}

float wsp::xerces::XmlHandler::GetFloatParameter(const std::string &param_name) const
{
    return float_param_list_.at(param_name);
}

std::string wsp::xerces::XmlHandler::GetStringParameter(const std::string &param_name) const
{
    return str_param_list_.at(param_name);
}

void wsp::xerces::XmlHandler::AddBoolParameter(const std::string &param_name, bool value)
{
    bool_param_list_.insert(std::map<std::string, bool>::value_type(param_name, value));
}

void wsp::xerces::XmlHandler::AddIntParameter(const std::string &param_name, int value)
{
    param_list_.insert(std::map<std::string, int>::value_type(param_name, value));
}

void wsp::xerces::XmlHandler::AddFloatParamter(const std::string &param_name, float value)
{
    float_param_list_.insert(std::map<std::string, float>::value_type(param_name, value));
}

void wsp::xerces::XmlHandler::AddStringParameter(const std::string &param_name, const std::string &value)
{
    str_param_list_.insert(std::map<std::string, std::string>::value_type(param_name, value));
}

// I/O ----------------------------------------------------
wsp::State wsp::xerces::XmlHandler::LoadParametersFromXml(const char *in_xmlPath)
{
    param_list_.clear();
    str_param_list_.clear();

    try{
        XMLPlatformUtils::Initialize();
    }catch(const XMLException& e){
        char* message = XMLString::transcode(e.getMessage());
        WSP_COMMON_ERROR_LOG("Error during initialization! : %s\n",message);
        XMLString::release(&message);
        return WSP_STATE_FAILURE;
    }
    XercesDOMParser* parser = new XercesDOMParser();

    ErrorHandler* errHandler = (ErrorHandler*)WSP_NEW HandlerBase();
    parser->setErrorHandler(errHandler);


    parser->parse(in_xmlPath);

    DOMDocument* doc = parser->getDocument();
    DOMElement * root = doc->getDocumentElement();

    DOMNodeList* pNodeList = root->getChildNodes();
    int length = pNodeList->getLength();

    for(int i=0; i<length; i++){
        DOMNode *pDomNode = (DOMNode *)pNodeList->item(i);
        if(pDomNode->hasAttributes()){
            DOMNamedNodeMap* pNamedNodeMap = pDomNode->getAttributes();
            char *param_name =NULL;
            char *paramValue=NULL;
            for(int i =0; i < pNamedNodeMap->getLength();++i)
            {
                DOMNode* pAttributeNode = pNamedNodeMap->item(i);
                char *attrName = XMLString::transcode(pAttributeNode->getNodeName());
                if(strcmp(attrName, "name")==0)
                {
                    param_name = XMLString::transcode(pAttributeNode->getTextContent());
                }
                else if(strcmp(attrName, "value")==0)
                {
                    paramValue = XMLString::transcode(pAttributeNode->getTextContent());
                }
                XMLString::release(&attrName);
            }
            char *c_ptr = paramValue;
            char *end_ptr = c_ptr + strlen( paramValue );
            for(; c_ptr!=end_ptr; ++c_ptr){
                if( isdigit(*c_ptr) ){ continue; }
            }
            if( c_ptr==end_ptr ) /* If paramValue is digit string */
            {
                int value = atoi(paramValue);
                param_list_.insert( map<string, int>::value_type(std::string(param_name), value) );
                printf("%s: (int)%d\n", param_name, value);
            }
            else
            {
                str_param_list_.insert( map<string, string>::value_type(std::string(param_name), std::string(paramValue)) );
                printf("%s: (string)%s\n", param_name, paramValue);
            }
            XMLString::release(&param_name);
            XMLString::release(&paramValue);
        }
    }
    delete parser;
    delete errHandler;

    XMLPlatformUtils::Terminate();

    return WSP_STATE_SUCCESS;
}



wsp::State wsp::xerces::XmlHandler::SaveParametersAsXml(const char *in_xmlPath)
{
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch(const XMLException& e)
    {
        char* message = XMLString::transcode(e.getMessage());
        WSP_COMMON_ERROR_LOG("Error during initialization! : %s\n",message);
        XMLString::release(&message);
        return WSP_STATE_FAILURE;
    }

    DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("Core"));

    if (impl != NULL)
    {
        try
        {
            DOMDocument* doc = impl->createDocument(
                0,                    // root element namespace URI.
                XMLString::transcode("ParameterSettings"),         // root element name
                0);                   // document type object (DTD).

            DOMElement* rootElem = doc->getDocumentElement();

            printf("param_list_.size()=%zu, str_param_list_.size()=%zu\n", param_list_.size(), str_param_list_.size());

            map<string, int>::iterator it = param_list_.begin();
            char vstr[MAX_LENGTH_OF_NAME];
            while( it != param_list_.end() )
            {
                //cout << (*it).first << ":" << (*it).second << endl;
                DOMElement *elem = doc->createElement(XMLString::transcode("param"));
                sprintf(vstr, "%d", (*it).second);
                //! set attribute
                elem->setAttribute(XMLString::transcode("name"), XMLString::transcode((*it).first.c_str()));
                elem->setAttribute(XMLString::transcode("value"),XMLString::transcode(vstr));

                rootElem->appendChild(elem);
                ++it;
            }

            map<string, string>::iterator it_s = str_param_list_.begin();
            while( it_s != str_param_list_.end() )
            {
                //cout << (*it).first << ":" << (*it).second << endl;
                DOMElement *elem = doc->createElement(XMLString::transcode("param"));
                //! set attribute
                elem->setAttribute(XMLString::transcode("name"), XMLString::transcode((*it_s).first.c_str()));
                elem->setAttribute(XMLString::transcode("value"),XMLString::transcode((*it_s).second.c_str()));

                rootElem->appendChild(elem);
                ++it_s;
            }

            //const XMLSize_t elementCount = doc->getElementsByTagName(XMLString::transcode("*"))->getLength();
            //XERCES_STD_QUALIFIER cout << "The tree just created contains: " << elementCount<< " elements." << XERCES_STD_QUALIFIER endl;


            wsp::xerces::OutputXML(doc, in_xmlPath);

            doc->release();

            printf("%s successfully saved\n", in_xmlPath);
        }
        catch (const OutOfMemoryException&)
        {
            XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
        }
        catch (const DOMException& e)
        {
            XERCES_STD_QUALIFIER cerr << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;
        }
        catch (...)
        {
            XERCES_STD_QUALIFIER cerr << "An error occurred creating the document" << XERCES_STD_QUALIFIER endl;
        }

    }  // (inpl != NULL)
    else
    {
        XERCES_STD_QUALIFIER cerr << "Requested implementation is not supported" << XERCES_STD_QUALIFIER endl;
    }

    XMLPlatformUtils::Terminate();

    return WSP_STATE_SUCCESS;
}
