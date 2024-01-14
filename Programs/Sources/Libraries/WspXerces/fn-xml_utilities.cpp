/**
 * @file fn-string.cpp
 * 
 */

#ifndef __WSP_COMMONUTIL_FUNCTION_STRING_H__
#include <wsp/xerces/fn-xml_utilities.h>
#endif

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

XERCES_CPP_NAMESPACE_USE

wsp::State wsp::xerces::OutputXML(xercesc::DOMDocument* pmyDOMDocument, const char *filePath)
{
    //! Return the first registered implementation that has the desired features. In this case, we are after a DOM implementation that has the LS feature... or Load/Save.
    DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(L"LS");

    //! Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document
    DOMLSSerializer *serializer = ((DOMImplementationLS*)implementation)->createLSSerializer();

    //! Make the output more human readable by inserting line feeds
    if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true)){
        serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    }

    //! The end-of-line sequence of characters to be used in the XML being written out
    serializer->setNewLine(XMLString::transcode("\r\n")); 

    //! Convert the path into Xerces compatible XMLCh*.
    XMLCh *tempFilePath = XMLString::transcode(filePath);

    //! Specify the target for the XML output.
    XMLFormatTarget *formatTarget = new LocalFileFormatTarget(tempFilePath);

    //! Create a new empty output destination object.
    DOMLSOutput *output = ((DOMImplementationLS*)implementation)->createLSOutput();

    //! Set the stream to our target.
    output->setByteStream(formatTarget);

    //! Write the serialized output to the destination.
    serializer->write(pmyDOMDocument, output);

    //! Cleanup
    serializer->release();
    XMLString::release(&tempFilePath);
    delete formatTarget;
    output->release();

    return WSP_STATE_SUCCESS;
}
