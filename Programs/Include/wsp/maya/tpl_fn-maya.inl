/**
 * @file tpl_fn-maya.inl
 * @author Jun-ichi Nishikata
 * @brief OpenMaya wrapper template functions.
 */

#ifndef WSP_MAYA_TEMPLATE_FUNCTION_MAYA_INL__
#define WSP_MAYA_TEMPLATE_FUNCTION_MAYA_INL__

#include <maya/MObject.h>
#include <maya/MVector.h>
#include <maya/MFnTransform.h>
#include <maya/MFnDagNode.h>

template<typename T> 
void wsp::maya::CreateLocator(const T in_pos[3])
{
    MFnDagNode fn_dag;
    MObject locator = fn_dag.create(MString("locator"));
    MFnTransform fn_transform(locator);
    fn_transform.setTranslation(
        MVector((double)in_pos[0], (double)in_pos[1], (double)in_pos[2]), 
        MSpace::kTransform
    );
}

#endif
