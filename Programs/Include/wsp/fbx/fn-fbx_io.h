#pragma once

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include <wsp/common/_define_commoncore.h>
#endif

#include "fn-fbx_log.h"
#include <wsp/common/common_types.h>
#include <wsp/graphics/RenderGeometry.h>
#include <wsp/graphics/RenderSkeleton.h>
#include <wsp/graphics/SkeletalAnim.h>
#include <wsp/graphics/WavefrontObj.h>
#include <wsp/graphics/fn-graphic_io.h>

namespace wsp{ namespace fbx{
    enum FbxResult {
        FBX_RESULT_NO_ERROR,
        FBX_RESULT_FAILED_TO_INITIALIZE_IMPORTER,
        FBX_RESULT_FAILED_TO_IMPORT_SCENE,
        FBX_RESULT_UNKOWN_ERROR,
    };

    WSP_DLL_EXPORT
        FbxResult LoadFbx(wsp::graphic::Obj* o_geom, const char* file_path);

    WSP_DLL_EXPORT
        FbxResult LoadFbx(wsp::graphic::Obj* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, const char* file_path);

	WSP_DLL_EXPORT
		FbxResult LoadFbx(wsp::graphic::Obj* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, wsp::graphic::SkeletalAnim* o_skeletal_anim, const char* file_path);

	WSP_DLL_EXPORT
        FbxResult LoadFbx(wsp::graphic::RenderGeometry* o_geom, const char* file_path);

    WSP_DLL_EXPORT
        FbxResult LoadFbx(wsp::graphic::RenderGeometry* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, wsp::graphic::SkeletalAnim* o_skeletal_anim, const char* file_path);

	WSP_DLL_EXPORT
		FbxResult LoadFbx(wsp::graphic::RenderGeometry* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, const char* file_path);

    WSP_DLL_EXPORT
        FbxResult LoadFbx(wsp::Vector<wsp::graphic::Obj*>* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, wsp::graphic::SkeletalAnim* o_skeletal_anim, const char* file_path);

    WSP_DLL_EXPORT
        FbxResult LoadFbx(wsp::Vector<wsp::graphic::RenderGeometry*>* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, wsp::graphic::SkeletalAnim* o_skeletal_anim, const char* file_path);

    inline void LoadFbxSequence(std::vector<wsp::graphic::RenderGeometry*>* o_geometries, const char* file_path)
    {
        char path[MAX_LENGTH_OF_PATH];
        wsp::SeqInfo sequence_info;
        wsp::GetSeqInfo(sequence_info, file_path);
        for (int frame = sequence_info.start_frame; frame <= sequence_info.end_frame; ++frame)
        {
            wsp::GetSeqPath(path, sequence_info, frame);
            wsp::graphic::RenderGeometry* geom = WSP_NEW wsp::graphic::RenderGeometry();
            WSP_FBX_LOG("Loading %s\r", path);
            FbxResult result = wsp::fbx::LoadFbx(geom, path);
            WSP_ASSERT(result == wsp::fbx::FBX_RESULT_NO_ERROR, "loading file failed: %s", path);
            o_geometries->push_back(geom);
        }

        WSP_FBX_LOG("\n");
    }
}}
