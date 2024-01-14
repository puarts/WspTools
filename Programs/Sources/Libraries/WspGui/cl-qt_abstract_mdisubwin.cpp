/**
 * @file wspfx-abst_mdisubwin.cpp
 * @author Jun-ichi Nishikata
 */

#include <vector>

#include <wsp/common/_define_commoncore.h>
#include <wsp/common/_ctypes.h>

namespace wsp{
    namespace gui{
        WSP_DLL_EXPORT std::vector<s32> g_window_stack_ = std::vector<s32>();
    }
}
