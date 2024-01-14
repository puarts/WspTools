
#include <wsp/xerces/XercesWrapper.h>


#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include <stdio.h>

XERCES_CPP_NAMESPACE_USE;

using namespace wsp::xerces;

XercesWrapper::XercesWrapper()
    : xerces_initialized_(false)
    , responsible_to_release_xdoc_(false)
    , parser_(NULL)
    , err_handler_(NULL)
    , xdoc_(NULL)
    , root_elem_(NULL)
    , dom_impl_(NULL)
{
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& e)
    {
        char* message = XMLString::transcode(e.getMessage());
        fprintf(stderr, "Error during initialization! : %s\n", message);
        XMLString::release(&message);
        return;
    }
    catch (...)
    {
        fprintf(stderr, "Unexpected Exception \n");
        return;
    }
    xerces_initialized_ = true;

    dom_impl_ = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("Core"));
    err_handler_ = (ErrorHandler*)WSP_NEW HandlerBase();
    parser_ = new XercesDOMParser();
    parser_->setErrorHandler(err_handler_);
}

XercesWrapper::~XercesWrapper()
{
    if (xerces_initialized_)
    {
        if (xdoc_ != NULL && responsible_to_release_xdoc_)
        {
            xdoc_->release();
        }
        delete parser_;
        delete err_handler_;

        XMLPlatformUtils::Terminate();
    }
}

XercesWrapper::Result XercesWrapper::Deserialize(const char* xml_path)
{
    if (xerces_initialized_ == false) { return XercesWrapper::XERCES_RESULT_NOT_INITIALIZED; }
    try
    {
        parser_->parse(xml_path);
    }
    catch (const XMLException& e)
    {
        char* message = XMLString::transcode(e.getMessage());
        fprintf(stderr, "Exception message is: %s\n", message);
        XMLString::release(&message);
        return XercesWrapper::XERCES_RESULT_XML_ERROR;
    }
    catch (const DOMException& e)
    {
        char* message = XMLString::transcode(e.msg);
        fprintf(stderr, "Exception message is: %s\n", message);
        XMLString::release(&message);
        return XercesWrapper::XERCES_RESULT_DOM_ERROR;
    }
    catch (...)
    {
        fprintf(stderr, "Unexpected Exception \n");
        return XercesWrapper::XERCES_RESULT_UNKNOWN_ERROR;
    }
    xdoc_ = parser_->getDocument();
    root_elem_ = xdoc_->getDocumentElement();

    return XercesWrapper::XERCES_RESULT_SUCCESS;
}

XercesWrapper::Result XercesWrapper::CreateDocument(const char* root_name)
{
    if (xerces_initialized_ == false) { return XercesWrapper::XERCES_RESULT_NOT_INITIALIZED; }
    if (dom_impl_ == NULL) { return XercesWrapper::XERCES_RESULT_NO_DOM_IMPLEMENTATION; }

    xdoc_ = dom_impl_->createDocument(
        0,                    // root element namespace URI.
        XMLString::transcode(root_name),  // root element name
        0);                   // document type object (DTD).

    root_elem_ = xdoc_->getDocumentElement();

    responsible_to_release_xdoc_ = true;

    return XercesWrapper::XERCES_RESULT_SUCCESS;
}

XercesWrapper::Result XercesWrapper::Serialize(const char* ouput_xml_path)
{
    if (xerces_initialized_ == false) { return XercesWrapper::XERCES_RESULT_NOT_INITIALIZED; }
    if (xdoc_ == NULL) { return XERCES_RESULT_DOCUMENT_IS_NULL; }

    DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(L"LS");
    DOMLSSerializer *serializer = ((DOMImplementationLS*)implementation)->createLSSerializer();
    if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
    {
        serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    }
    serializer->setNewLine(XMLString::transcode("\r\n"));
    XMLCh *tempFilePath = XMLString::transcode(ouput_xml_path);
    XMLFormatTarget *formatTarget = new LocalFileFormatTarget(tempFilePath);
    DOMLSOutput *output = ((DOMImplementationLS*)implementation)->createLSOutput();
    output->setByteStream(formatTarget);
    serializer->write(xdoc_, output);

    serializer->release();
    XMLString::release(&tempFilePath);
    delete formatTarget;
    output->release();

    return XercesWrapper::XERCES_RESULT_SUCCESS;
}

DOMElement* XercesWrapper::CreateElement(const char* elem_name)
{
    if (xdoc_ == NULL) { return NULL; }

    return xdoc_->createElement(XMLString::transcode(elem_name));
}
