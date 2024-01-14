#pragma once

#include <wsp/common/_define_commoncore.h>
#include <wsp/common/common_types.h>
#include <xercesc/dom/DOM.hpp>

namespace wsp{
    namespace xerces{
        WSP_DLL_EXPORT wsp::State OutputXML(xercesc::DOMDocument* pmyDOMDocument, const char *filePath);
    }
}
