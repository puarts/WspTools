#pragma once

#include <wsp/common/_define_commoncore.h>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>

namespace wsp {
    namespace xerces {
        class WSP_DLL_EXPORT XercesWrapper
        {
        public:
            enum Result {
                XERCES_RESULT_SUCCESS,
                XERCES_RESULT_NOT_INITIALIZED,
                XERCES_RESULT_XML_ERROR,
                XERCES_RESULT_DOM_ERROR,
                XERCES_RESULT_NO_DOM_IMPLEMENTATION,
                XERCES_RESULT_DOCUMENT_IS_NULL,
                XERCES_RESULT_UNKNOWN_ERROR
            };

        public:
            XercesWrapper();
            ~XercesWrapper();

            bool xerces_initialized() const { return xerces_initialized_; }
            XERCES_CPP_NAMESPACE::DOMDocument* xdoc() { return xdoc_; }
            XERCES_CPP_NAMESPACE::DOMElement*  root_elem() { return root_elem_; }
            XERCES_CPP_NAMESPACE::DOMImplementation* dom_impl() { return dom_impl_; }

            XercesWrapper::Result CreateDocument(const char* root_name);

            XercesWrapper::Result Serialize(const char* ouput_xml_path);
            XercesWrapper::Result Deserialize(const char* xml_path);

            XERCES_CPP_NAMESPACE::DOMElement* CreateElement(const char* elem_name);

        private:
            XercesWrapper(const XercesWrapper &rhs);
            XercesWrapper &operator=(const XercesWrapper &rhs);

        private:
            XERCES_CPP_NAMESPACE::XercesDOMParser*   parser_;
            XERCES_CPP_NAMESPACE::ErrorHandler*      err_handler_;
            XERCES_CPP_NAMESPACE::DOMDocument*       xdoc_;
            XERCES_CPP_NAMESPACE::DOMElement*        root_elem_;
            XERCES_CPP_NAMESPACE::DOMImplementation* dom_impl_;
            bool               xerces_initialized_;
            bool               responsible_to_release_xdoc_;
        };

    }
}
