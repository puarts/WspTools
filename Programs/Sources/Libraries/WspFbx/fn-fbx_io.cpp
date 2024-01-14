/**
 * @file fn-imgio-gpl.cpp
 * 
 */

#include <wsp/fbx/fn-fbx_io.h>
#include <wsp/fbx/Common.h>

using namespace std;

namespace{
	const int InvalidBoneIndex = -1;

    template<typename _Type>
    class ScopedPtrVector
    {
    public:
        ScopedPtrVector()
        {
        }

        ~ScopedPtrVector()
        {
            for (int i = 0; i < vec_.length(); ++i)
            {
                delete vec_[i];
            }
        }

        wsp::Vector<_Type*>* vector()
        {
            return &vec_;
        }

    private:
        wsp::Vector<_Type*> vec_;
    };


    void GetMesh(FbxNode* node)
    {
        // メッシュ
        FbxMesh* mesh = node->GetMesh();

        if (NULL != mesh)
            printf("\nMesh=%s Node=%s", mesh->GetName(), node->GetName());

        // 子ノード
        for (int i = 0; i < node->GetChildCount(); i++)
            GetMesh(node->GetChild(i));
    }

    static bool gVerbose = true;

	void DisplayAnimation(wsp::graphic::SkeletalAnim* o_skeletal_anim, FbxAnimStack* pAnimStack, FbxNode* pNode);
	void DisplayAnimation(wsp::graphic::SkeletalAnim* o_skeletal_anim, FbxAnimLayer* pAnimLayer, FbxNode* pNode);
	void DisplayChannels(
		wsp::graphic::SkeletalAnim* o_skeletal_anim, 
		FbxNode* pNode, FbxAnimLayer* pAnimLayer,
		void(*DisplayCurve) (wsp::graphic::AnimCurve<float>*, FbxAnimCurve* pCurve),
		void(*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty));
	void DisplayCurveKeys(
		wsp::graphic::AnimCurve<float>* o_key, FbxAnimCurve* pCurve);
	void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);


	void DisplayAnimation(wsp::graphic::SkeletalAnim* o_skeletal_anim, FbxScene* pScene)
	{
		int i;
		for (i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); i++)
		{
			FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

			FbxString lOutputString = "Animation Stack Name: ";
			lOutputString += lAnimStack->GetName();
			lOutputString += "\n\n";
			FBXSDK_printf(lOutputString);

			DisplayAnimation(o_skeletal_anim, lAnimStack, pScene->GetRootNode());
		}
	}

	void DisplayAnimation(wsp::graphic::SkeletalAnim* o_skeletal_anim, FbxAnimStack* pAnimStack, FbxNode* pNode)
	{
		int l;
		int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
		FbxString lOutputString;

		lOutputString = "Animation stack contains ";
		lOutputString += nbAnimLayers;
		lOutputString += " Animation Layer(s)\n";
		FBXSDK_printf(lOutputString);

		for (l = 0; l < nbAnimLayers; l++)
		{
			FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

			lOutputString = "AnimLayer ";
			lOutputString += l;
			lOutputString += "\n";
			FBXSDK_printf(lOutputString);

			DisplayAnimation(o_skeletal_anim, lAnimLayer, pNode);
		}
	}

	void DisplayAnimation(wsp::graphic::SkeletalAnim* o_skeletal_anim, FbxAnimLayer* pAnimLayer, FbxNode* pNode)
	{
		int lModelCount;
		FbxString lOutputString;

		lOutputString = "     Node Name: ";
		lOutputString += pNode->GetName();
		lOutputString += "\n\n";
		FBXSDK_printf(lOutputString);

		DisplayChannels(o_skeletal_anim, pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys);
		FBXSDK_printf("\n");

		for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
		{
			DisplayAnimation(o_skeletal_anim, pAnimLayer, pNode->GetChild(lModelCount));
		}
	}


	void DisplayChannels(
		wsp::graphic::SkeletalAnim* o_skeletal_anim,
		FbxNode* pNode,
		FbxAnimLayer* pAnimLayer,
		void(*DisplayCurve) (wsp::graphic::AnimCurve<float>*,FbxAnimCurve* pCurve),
		void(*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty))
	{
		FbxAnimCurve* lAnimCurve = NULL;

		wsp::graphic::BoneAnim bone_anim;
		bone_anim.set_bone_name(pNode->GetName());

		// Display general curves.
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			FBXSDK_printf("        TX\n");
			wsp::graphic::AnimCurve<float>& curve = bone_anim.translate_x_curve();
			DisplayCurve(&curve, lAnimCurve);
		}
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			FBXSDK_printf("        TY\n");
			wsp::graphic::AnimCurve<float>& curve = bone_anim.translate_y_curve();
			DisplayCurve(&curve, lAnimCurve);
		}
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			FBXSDK_printf("        TZ\n");
			wsp::graphic::AnimCurve<float>& curve = bone_anim.translate_z_curve();
			DisplayCurve(&curve, lAnimCurve);
		}

		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			FBXSDK_printf("        RX\n");
			wsp::graphic::AnimCurve<float>& curve = bone_anim.rotate_x_curve();
			DisplayCurve(&curve, lAnimCurve);
		}
		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			FBXSDK_printf("        RY\n");
			wsp::graphic::AnimCurve<float>& curve = bone_anim.rotate_y_curve();
			DisplayCurve(&curve, lAnimCurve);
		}
		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			FBXSDK_printf("        RZ\n");
			wsp::graphic::AnimCurve<float>& curve = bone_anim.rotate_z_curve();
			DisplayCurve(&curve, lAnimCurve);
		}

		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			FBXSDK_printf("        SX\n");
			wsp::graphic::AnimCurve<float>& curve = bone_anim.scale_x_curve();
			DisplayCurve(&curve, lAnimCurve);
		}
		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			FBXSDK_printf("        SY\n");
			wsp::graphic::AnimCurve<float>& curve = bone_anim.scale_y_curve();
			DisplayCurve(&curve, lAnimCurve);
		}
		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			FBXSDK_printf("        SZ\n");
			wsp::graphic::AnimCurve<float>& curve = bone_anim.scale_z_curve();
			DisplayCurve(&curve, lAnimCurve);
		}

		o_skeletal_anim->AddBoneAnim(bone_anim);
	}


	static int InterpolationFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant) return 1;
		if ((flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear) return 2;
		if ((flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic) return 3;
		return 0;
	}

	static int ConstantmodeFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard) return 1;
		if ((flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext) return 2;
		return 0;
	}

	static int TangentmodeFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto) return 1;
		if ((flags & FbxAnimCurveDef::eTangentAutoBreak) == FbxAnimCurveDef::eTangentAutoBreak) return 2;
		if ((flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB) return 3;
		if ((flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser) return 4;
		if ((flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak) return 5;
		if ((flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak) return 6;
		return 0;
	}

	static int TangentweightFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone) return 1;
		if ((flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight) return 2;
		if ((flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft) return 3;
		return 0;
	}

	static int TangentVelocityFlagToIndex(int flags)
	{
		if ((flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone) return 1;
		if ((flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight) return 2;
		if ((flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft) return 3;
		return 0;
	}

	void DisplayCurveKeys(wsp::graphic::AnimCurve<float>* o_curve, FbxAnimCurve* pCurve)
	{
		static const char* interpolation[] = { "?", "constant", "linear", "cubic" };
		static const char* constantMode[] = { "?", "Standard", "Next" };
		static const char* cubicMode[] = { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
		static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

		fbxsdk::FbxTime   lKeyTime;
		float   lKeyValue;
		char    lTimeString[256];
		FbxString lOutputString;
		int     lCount;

		int lKeyCount = pCurve->KeyGetCount();

		for (lCount = 0; lCount < lKeyCount; lCount++)
		{
			lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
			lKeyTime = pCurve->KeyGetTime(lCount);
			o_curve->AddKey(lKeyValue, (float)lKeyTime.GetFrameCount());

			lOutputString = "            Key Time: ";
			lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
			lOutputString += ".... Key Value: ";
			lOutputString += lKeyValue;
			lOutputString += " [ ";
			lOutputString += interpolation[InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount))];
			if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
			{
				lOutputString += " | ";
				lOutputString += constantMode[ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount))];
			}
			else if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
			{
				lOutputString += " | ";
				lOutputString += cubicMode[TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount))];
				lOutputString += " | ";
				lOutputString += tangentWVMode[TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode())];
				lOutputString += " | ";
				lOutputString += tangentWVMode[TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode())];
			}
			lOutputString += " ]";
			lOutputString += "\n";
			FBXSDK_printf(lOutputString);
		}
	}

	void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
	{
		fbxsdk::FbxTime   lKeyTime;
		int     lKeyValue;
		char    lTimeString[256];
		FbxString lListValue;
		FbxString lOutputString;
		int     lCount;

		int lKeyCount = pCurve->KeyGetCount();

		for (lCount = 0; lCount < lKeyCount; lCount++)
		{
			lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
			lKeyTime = pCurve->KeyGetTime(lCount);

			lOutputString = "            Key Time: ";
			lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
			lOutputString += ".... Key Value: ";
			lOutputString += lKeyValue;
			lOutputString += " (";
			lOutputString += pProperty->GetEnumValue(lKeyValue);
			lOutputString += ")";

			lOutputString += "\n";
			FBXSDK_printf(lOutputString);
		}
	}

    wsp::graphic::RenderBone AddBone(wsp::graphic::RenderSkeleton* o_skeleton, FbxNode* node)
    {
        fbxsdk::FbxDouble3 local_translate = node->LclTranslation.Get();
        fbxsdk::FbxDouble3 local_rotate = node->LclRotation.Get();
        fbxsdk::FbxDouble3 local_scale = node->LclScaling.Get();
        fbxsdk::FbxVector4 rot_pivot = node->GetRotationPivot(fbxsdk::FbxNode::EPivotSet::eSourcePivot);
        //fbxsdk::FbxVector4 pre_rotate = node->GetPreRotation(fbxsdk::FbxNode::EPivotSet::eDestinationPivot);
        //fbxsdk::FbxVector4 post_rotate = node->GetPostRotation(fbxsdk::FbxNode::EPivotSet::eDestinationPivot);
        fbxsdk::FbxVector4 pre_rotate = node->GetPreRotation(fbxsdk::FbxNode::EPivotSet::eSourcePivot);
        fbxsdk::FbxVector4 post_rotate = node->GetPostRotation(fbxsdk::FbxNode::EPivotSet::eSourcePivot);

        int bone_index = o_skeleton->GetBoneCount();
        wsp::graphic::RenderBone tmp_bone(
            node->GetName(),
            node->GetParent() != nullptr ? node->GetParent()->GetName() : nullptr,
            local_translate[0] + rot_pivot[0],
            local_translate[1] + rot_pivot[1],
            local_translate[2] + rot_pivot[2],
            static_cast<float>(local_rotate[0] * M_PI / 180.0),
            static_cast<float>(local_rotate[1] * M_PI / 180.0),
            static_cast<float>(local_rotate[2] * M_PI / 180.0),
            local_scale[0],
            local_scale[1],
            local_scale[2],
            static_cast<float>(pre_rotate[0] * M_PI / 180.0),
            static_cast<float>(pre_rotate[1] * M_PI / 180.0),
            static_cast<float>(pre_rotate[2] * M_PI / 180.0),
            static_cast<float>(post_rotate[0] * M_PI / 180.0),
            static_cast<float>(post_rotate[1] * M_PI / 180.0),
            static_cast<float>(post_rotate[2] * M_PI / 180.0),
            bone_index);

        o_skeleton->AddBone(tmp_bone);
        return tmp_bone;
    }

    void AddBones(wsp::graphic::RenderSkeleton* o_skeleton, FbxNode* node)
    {
        wsp::graphic::RenderBone* bone = o_skeleton->FindBone(node->GetName());
        if (bone == nullptr)
        {
            AddBone(o_skeleton, node);
        }

        for (int child_index = 0, count = node->GetChildCount(); child_index < count; ++child_index)
        {
            AddBones(o_skeleton, node->GetChild(child_index));
        }
    }


    //get mesh smoothing info
    //set pCompute true to compute smoothing from normals by default 
    //set pConvertToSmoothingGroup true to convert hard/soft edge info to smoothing group info by default
    void GetSmoothing(FbxManager* pSdkManager, FbxNode* pNode, bool pCompute, bool pConvertToSmoothingGroup)
    {
        if (!pNode || !pSdkManager)
            return;

        //get mesh
        FbxMesh* mesh = pNode->GetMesh();
        if (mesh)
        {
            //print mesh node name
            FBXSDK_printf("current mesh node: %s\n", pNode->GetName());

            //if there's no smoothing info in fbx file, but you still want to get smoothing info.
            //please compute smoothing info from normals.
            //Another case to recompute smoothing info from normals is:
            //If users edit normals manually in 3ds Max or Maya and export the scene to FBX with smoothing info,
            //The smoothing info may NOT match with normals.
            //the mesh called "fbx_customNormals" in Normals.fbx is the case. All edges are hard, but normals actually represent the "soft" looking.
            //Generally, the normals in fbx file holds the smoothing result you'd like to get.
            //If you want to get correct smoothing info(smoothing group or hard/soft edges) which match with normals,
            //please drop the original smoothing info of fbx file, and recompute smoothing info from normals.
            //if you want to get soft/hard edge info, please call FbxGeometryConverter::ComputeEdgeSmoothingFromNormals().
            //if you want to get smoothing group info, please get soft/hard edge info first by ComputeEdgeSmoothingFromNormals() 
            //And then call FbxGeometryConverter::ComputePolygonSmoothingFromEdgeSmoothing().
            if (pCompute)
            {
                FbxGeometryConverter lGeometryConverter(pSdkManager);
                lGeometryConverter.ComputeEdgeSmoothingFromNormals(mesh);
                //convert soft/hard edge info to smoothing group info
                if (pConvertToSmoothingGroup)
                    lGeometryConverter.ComputePolygonSmoothingFromEdgeSmoothing(mesh);
            }

            //if there is smoothing groups info in your fbx file, but you want to get hard/soft edges info
            //please use following code:
            //FbxGeometryConverter lGeometryConverter(lSdkManager);
            //lGeometryConverter.ComputeEdgeSmoothingFromPolygonSmoothing(lMesh);

            //get smoothing info
            FbxGeometryElementSmoothing* lSmoothingElement = mesh->GetElementSmoothing();
            if (lSmoothingElement)
            {
                //mapping mode is by edge. The mesh usually come from Maya, because Maya can set hard/soft edges.
                //we can get smoothing info(which edges are soft, which edges are hard) by retrieving each edge. 
                if (lSmoothingElement->GetMappingMode() == FbxGeometryElement::eByEdge)
                {
                    //Let's get smoothing of each edge, since the mapping mode of smoothing element is by edge
                    for (int lEdgeIndex = 0; lEdgeIndex < mesh->GetMeshEdgeCount(); lEdgeIndex++)
                    {
                        int lSmoothingIndex = 0;
                        //reference mode is direct, the smoothing index is same as edge index.
                        //get smoothing by the index of edge
                        if (lSmoothingElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                            lSmoothingIndex = lEdgeIndex;

                        //reference mode is index-to-direct, get smoothing by the index-to-direct
                        if (lSmoothingElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                            lSmoothingIndex = lSmoothingElement->GetIndexArray().GetAt(lEdgeIndex);

                        //Got smoothing of each vertex.
                        int lSmoothingFlag = lSmoothingElement->GetDirectArray().GetAt(lSmoothingIndex);
                        if (gVerbose) FBXSDK_printf("hard/soft value for edge[%d]: %d \n", lEdgeIndex, lSmoothingFlag);
                        //add your custom code here, to output smoothing or get them into a list, such as KArrayTemplate<int>
                        //. . .
                    }//end for lEdgeIndex
                }//end eByEdge
                 //mapping mode is by polygon. The mesh usually come from 3ds Max, because 3ds Max can set smoothing groups for polygon.
                 //we can get smoothing info(smoothing group ID for each polygon) by retrieving each polygon. 
                else if (lSmoothingElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
                {
                    //Let's get smoothing of each polygon, since the mapping mode of smoothing element is by polygon.
                    for (int lPolygonIndex = 0; lPolygonIndex < mesh->GetPolygonCount(); lPolygonIndex++)
                    {
                        int lSmoothingIndex = 0;
                        //reference mode is direct, the smoothing index is same as polygon index.
                        if (lSmoothingElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                            lSmoothingIndex = lPolygonIndex;

                        //reference mode is index-to-direct, get smoothing by the index-to-direct
                        if (lSmoothingElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                            lSmoothingIndex = lSmoothingElement->GetIndexArray().GetAt(lPolygonIndex);

                        //Got smoothing of each polygon.
                        int lSmoothingFlag = lSmoothingElement->GetDirectArray().GetAt(lSmoothingIndex);
                        if (gVerbose) FBXSDK_printf("smoothing group ID for polygon[%d]: %d \n", lPolygonIndex, lSmoothingFlag);
                        //add your custom code here, to output normals or get them into a list, such as KArrayTemplate<int>
                        //. . .

                    }//end for lPolygonIndex //PolygonCount

                }//end eByPolygonVertex
            }//end if lSmoothingElement
        }//end if lMesh

         //recursively traverse each node in the scene
        int i, lCount = pNode->GetChildCount();
        for (i = 0; i < lCount; i++)
        {
            GetSmoothing(pSdkManager, pNode->GetChild(i), pCompute, pConvertToSmoothingGroup);
        }
    }

    void GetIndexList(wsp::Vector<int>* o_index_list, FbxMesh* mesh)
    {
        // あらかじめ三角形化してある
        auto polygon_count = mesh->GetPolygonCount();

        for (int i = 0; i < polygon_count; ++i)
        {
            int tri_vert0_index = mesh->GetPolygonVertex(i, 0);
            int tri_vert1_index = mesh->GetPolygonVertex(i, 1);
            int tri_vert2_index = mesh->GetPolygonVertex(i, 2);
            o_index_list->Append(tri_vert0_index);
            o_index_list->Append(tri_vert1_index);
            o_index_list->Append(tri_vert2_index);

            //FBXSDK_printf("index[%d]: %d \n",
            //    i * 3 + 0, tri_vert0_index);
            //FBXSDK_printf("index[%d]: %d \n",
            //    i * 3 + 1, tri_vert1_index);
            //FBXSDK_printf("index[%d]: %d \n",
            //    i * 3 + 2, tri_vert2_index);
        }
    }

    void GetPositionList(wsp::graphic::Obj* o_obj, FbxMesh* mesh)
    {
        o_obj->num_triangles = mesh->GetPolygonCount();
        o_obj->num_vertices = mesh->GetControlPointsCount();

        o_obj->triangle_vertex_position_indices.clear();
        o_obj->triangle_vertex_position_indices.reserve(o_obj->num_triangles * 3);
        for (int i = 0; i < o_obj->num_triangles; ++i)
        {
            int tri_vert0_index = mesh->GetPolygonVertex(i, 0);
            int tri_vert1_index = mesh->GetPolygonVertex(i, 1);
            int tri_vert2_index = mesh->GetPolygonVertex(i, 2);
            o_obj->triangle_vertex_position_indices.push_back(tri_vert0_index);
            o_obj->triangle_vertex_position_indices.push_back(tri_vert1_index);
            o_obj->triangle_vertex_position_indices.push_back(tri_vert2_index);

            //FBXSDK_printf("index[%d]: %d \n",
            //    i * 3 + 0, tri_vert0_index);
            //FBXSDK_printf("index[%d]: %d \n",
            //    i * 3 + 1, tri_vert1_index);
            //FBXSDK_printf("index[%d]: %d \n",
            //    i * 3 + 2, tri_vert2_index);
        }

        o_obj->vertex_positions.clear();
        o_obj->vertex_positions.reserve(o_obj->num_vertices);

        // コントロールポイントがいわゆる位置座標
        for (int index = 0, count = mesh->GetControlPointsCount(); index < count; ++index)
        {
            auto control_point = mesh->GetControlPointAt(index);

            o_obj->vertex_positions.push_back(WspVec3F(control_point[0], control_point[1], control_point[2]));
            //FBXSDK_printf("position[%d]: %f %f %f %f \n",
            //    index, control_point[0], control_point[1], control_point[2], control_point[3]);
            // wは0.0のみ対応
            WSP_ASSERT(control_point[3] == 0.0, "unsupported data");
        }
    }

    void GetPositionList(wsp::Vector<WspVec3F>* o_positions, FbxMesh* mesh, const wsp::Vector<int>& index_list)
    {
        // コントロールポイントがいわゆる位置座標
        for (int index = 0, count = index_list.length(); index < count; ++index)
        {
            auto control_point = mesh->GetControlPointAt(index_list[index]);

            o_positions->Append(WspVec3F(control_point[0], control_point[1], control_point[2]));
            FBXSDK_printf("position[%d]: %f %f %f %f \n",
                index, control_point[0], control_point[1], control_point[2], control_point[3]);
            // wは0.0のみ対応
            WSP_ASSERT(control_point[3] == 0.0, "unsupported data");
        }
    }

    void GetNormalList(wsp::Vector<WspVec3F>* o_normals, FbxMesh* mesh, const wsp::Vector<int>& indexList)
    {
        auto elementCount = mesh->GetElementNormalCount();

        // 法線要素が1のみ対応
        WSP_ASSERT(elementCount == 1, "unsupported data");

        auto element = mesh->GetElementNormal();
        auto mappingMode = element->GetMappingMode();
        auto referenceMode = element->GetReferenceMode();
        const auto& indexArray = element->GetIndexArray();
        const auto& directArray = element->GetDirectArray();

        // eDirctかeIndexDirectのみ対応
        WSP_ASSERT((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect), "Unsupported data");

        if (mappingMode == FbxGeometryElement::eByControlPoint)
        {
            // コントロールポイントでマッピング
            for (int index = 0, count = indexList.length(); index < count; ++index)
            {
                auto normalIndex = (referenceMode == FbxGeometryElement::eDirect)
                    ? index
                    : indexArray.GetAt(index);
                auto normal = directArray.GetAt(normalIndex);
                o_normals->Append(WspVec3F(normal[0], normal[1], normal[2]));
                FBXSDK_printf("normal[%d]: %f %f %f \n",
                    index, normal[0], normal[1], normal[2]);
                // wは1.0のみ対応
                WSP_ASSERT(normal[3] == 1.0, "Unsupported data");
            }
        }
        else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
        {
            // ポリゴンバーテックス（インデックス）でマッピング
            int indexByPolygonVertex = 0;
            int polygonCount = mesh->GetPolygonCount();
            for (int i = 0; i < polygonCount; ++i)
            {
                int polygonSize = mesh->GetPolygonSize(i);

                for (int j = 0; j < polygonSize; ++j)
                {
                    auto normalIndex = (referenceMode == FbxGeometryElement::eDirect)
                        ? indexByPolygonVertex
                        : indexArray.GetAt(indexByPolygonVertex);
                    auto normal = directArray.GetAt(normalIndex);

                    o_normals->Append(WspVec3F(normal[0], normal[1], normal[2]));
                    FBXSDK_printf("normal[%d]: %f %f %f \n",
                        o_normals->length() - 1, normal[0], normal[1], normal[2]);
                    // wは1.0のみ対応
                    WSP_ASSERT(normal[3] == 1.0, "Unsupported data");

                    ++indexByPolygonVertex;
                }

            }
        }
        else
        {
            // それ以外のマッピングモードには対応しない
            WSP_FATAL_UNEXPECTED_DEFAULT;
        }
    }


    void GetNormalList(wsp::graphic::Obj* o_obj, FbxMesh* mesh)
    {
        int element_count = mesh->GetElementNormalCount();

        WSP_ASSERT(element_count == 1, "unsupported data");

        auto element = mesh->GetElementNormal();
        FbxGeometryElement::EMappingMode mappingMode = element->GetMappingMode();
        FbxGeometryElement::EReferenceMode referenceMode = element->GetReferenceMode();
        const auto& index_array = element->GetIndexArray();
        const auto& direct_array = element->GetDirectArray();

        o_obj->triangle_normal_indices.clear();
        o_obj->normals.clear();

        o_obj->num_normals = direct_array.GetCount();
        o_obj->num_triangles = mesh->GetPolygonCount();

        o_obj->normals.reserve(o_obj->num_normals);
        for (int index = 0, count = direct_array.GetCount(); index < count; ++index)
        {
            auto normal = direct_array.GetAt(index);
            o_obj->normals.push_back(WspVec3F(normal[0], normal[1], normal[2]));
        }

        int normal_index_count = o_obj->num_triangles * 3;
        o_obj->triangle_normal_indices.reserve(normal_index_count);
        switch (mappingMode)
        {
        case FbxGeometryElement::eByControlPoint:
        {
            switch (referenceMode)
            {
            case FbxGeometryElement::eDirect:
            {
                for (int index = 0; index < normal_index_count; ++index)
                {
                    o_obj->triangle_normal_indices.push_back(index);
                }
            }
            break;
            case FbxGeometryElement::eIndexToDirect:
            {
                WSP_ASSERT(normal_index_count == index_array.GetCount(), "normal index count mismatch");
                for (int index = 0, count = index_array.GetCount(); index < count; ++index)
                {
                    int normal_index = index_array.GetAt(index);
                    o_obj->triangle_normal_indices.push_back(normal_index);
                }
            }
            break;
            default:
                WSP_FATAL_UNEXPECTED_DEFAULT;
            }
        }
        break;
        case FbxGeometryElement::eByPolygonVertex:
        {
            auto index_by_polygon_vertex = 0;
            auto polygon_count = mesh->GetPolygonCount();

            switch (referenceMode)
            {
            case FbxGeometryElement::eDirect:
            {
                for (int i = 0; i < polygon_count; ++i)
                {
                    for (int j = 0, vtx_count_on_polygon = mesh->GetPolygonSize(i); j < vtx_count_on_polygon; ++j, ++index_by_polygon_vertex)
                    {
                        o_obj->triangle_normal_indices.push_back(index_by_polygon_vertex);
                    }
                }
            }
            break;
            case FbxGeometryElement::eIndexToDirect:
            {
                int indexByPolygonVertex = 0;
                int polygonCount = mesh->GetPolygonCount();
                for (int i = 0; i < polygonCount; ++i)
                {
                    int polygonSize = mesh->GetPolygonSize(i);
                    for (int j = 0; j < polygonSize; ++j)
                    {
                        int normal_index = index_array.GetAt(indexByPolygonVertex);
                        o_obj->triangle_normal_indices.push_back(normal_index);
                        ++indexByPolygonVertex;
                    }
                }

                //for (int index = 0, count = index_array.GetCount(); index < count; ++index)
                //{
                //    int normal_index = index_array.GetAt(index);
                //    o_obj->triangle_normal_indices.push_back(normal_index);
                //}
            }
            break;
            default:
                WSP_FATAL_UNEXPECTED_DEFAULT;
            }
        }
        break;
        default:
            WSP_FATAL_UNEXPECTED_DEFAULT;
        }
    }

    void GetTextureCoordinates(wsp::graphic::Obj* o_obj, FbxMesh* mesh)
    {
        int element_count = mesh->GetElementUVCount();

        //WSP_ASSERT(element_count == 1, "unsupported data");

        auto element = mesh->GetElementUV();
        FbxGeometryElement::EMappingMode mappingMode = element->GetMappingMode();
        FbxGeometryElement::EReferenceMode referenceMode = element->GetReferenceMode();
        const auto& index_array = element->GetIndexArray();
        const auto& direct_array = element->GetDirectArray();

        o_obj->triangle_tex_coord_indices.clear();
        o_obj->tex_coords.clear();

        o_obj->num_tex_coords = direct_array.GetCount();
        o_obj->num_triangles = mesh->GetPolygonCount();

        o_obj->tex_coords.reserve(o_obj->num_tex_coords);
        for (int index = 0, count = direct_array.GetCount(); index < count; ++index)
        {
            auto uv = direct_array.GetAt(index);
            o_obj->tex_coords.push_back(WspVec2F(uv[0], uv[1]));
        }

        int tex_coord_index_count = o_obj->num_triangles * 3;
        o_obj->triangle_tex_coord_indices.reserve(tex_coord_index_count);
        switch (mappingMode)
        {
        case FbxGeometryElement::eByControlPoint:
        {
            switch (referenceMode)
            {
            case FbxGeometryElement::eDirect:
            {
                for (int index = 0; index < tex_coord_index_count; ++index)
                {
                    o_obj->triangle_tex_coord_indices.push_back(index);
                }
            }
            break;
            case FbxGeometryElement::eIndexToDirect:
            {
                WSP_ASSERT(tex_coord_index_count == index_array.GetCount(), "tex coord index count mismatch");
                for (int index = 0, count = index_array.GetCount(); index < count; ++index)
                {
                    int tex_coord_index = index_array.GetAt(index);
                    o_obj->triangle_tex_coord_indices.push_back(tex_coord_index);
                }
            }
            break;
            default:
                WSP_FATAL_UNEXPECTED_DEFAULT;
            }
        }
        break;
        case FbxGeometryElement::eByPolygonVertex:
        {
            int index_by_polygon_vertex = 0;
            int polygon_count = mesh->GetPolygonCount();

            switch (referenceMode)
            {
            case FbxGeometryElement::eDirect:
            {
                for (int i = 0; i < polygon_count; ++i)
                {
                    for (int j = 0, vtx_count_on_polygon = mesh->GetPolygonSize(i); j < vtx_count_on_polygon; ++j, ++index_by_polygon_vertex)
                    {
                        o_obj->triangle_tex_coord_indices.push_back(index_by_polygon_vertex);
                    }
                }
            }
            break;
            case FbxGeometryElement::eIndexToDirect:
            {
                int indexByPolygonVertex = 0;
                int polygonCount = mesh->GetPolygonCount();
                for (int i = 0; i < polygonCount; ++i)
                {
                    int polygonSize = mesh->GetPolygonSize(i);
                    for (int j = 0; j < polygonSize; ++j)
                    {
                        int tex_coord_index = index_array.GetAt(indexByPolygonVertex);
                        WSP_ASSERT(tex_coord_index != -1, "invalid uv index");
                        o_obj->triangle_tex_coord_indices.push_back(tex_coord_index);
                        ++indexByPolygonVertex;
                    }
                }

                //for (int index = 0, count = index_array.GetCount(); index < count; ++index)
                //{
                //    int tex_coord_index = index_array.GetAt(index);
                //    o_obj->triangle_tex_coord_indices.push_back(tex_coord_index);
                //}
            }
            break;
            default:
                WSP_FATAL_UNEXPECTED_DEFAULT;
            }
        }
        break;
        default:
            WSP_FATAL_UNEXPECTED_DEFAULT;
        }
    }

    //get mesh normals info
    void GetNormals(wsp::graphic::RenderGeometry* o_geom, FbxNode* node)
    {
        if (!node)
        {
            return;
        }

        //get mesh
        FbxMesh* mesh = node->GetMesh();
        if (mesh)
        {
            //print mesh node name
            FBXSDK_printf("current mesh node: %s\n", node->GetName());

            //{
            //    //get the normal element
            //    FbxGeometryElementNormal* normal_elem = mesh->GetElementNormal();
            //    if (normal_elem)
            //    {
            //        //mapping mode is by control points. The mesh should be smooth and soft.
            //        //we can get normals by retrieving each control point
            //        if (normal_elem->GetMappingMode() == FbxGeometryElement::eByControlPoint)
            //        {
            //            //Let's get normals of each vertex, since the mapping mode of normal element is by control point
            //            for (int vertex_index = 0; vertex_index < mesh->GetControlPointsCount(); vertex_index++)
            //            {
            //                int normal_index = 0;
            //                //reference mode is direct, the normal index is same as vertex index.
            //                //get normals by the index of control vertex
            //                if (normal_elem->GetReferenceMode() == FbxGeometryElement::eDirect)
            //                    normal_index = vertex_index;

            //                //reference mode is index-to-direct, get normals by the index-to-direct
            //                if (normal_elem->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
            //                    normal_index = normal_elem->GetIndexArray().GetAt(vertex_index);

            //                //Got normals of each vertex.
            //                FbxVector4 normal = normal_elem->GetDirectArray().GetAt(normal_index);
            //                if (gVerbose)
            //                {
            //                    FBXSDK_printf("normals for vertex[%d]: %f %f %f %f \n", vertex_index, normal[0], normal[1], normal[2], normal[3]);
            //                }

            //                o_geom->vertex_normals.Append(WspVec3F(normal[0], normal[1], normal[2]));
            //                //add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
            //                //. . .
            //            }//end for lVertexIndex
            //        }//end eByControlPoint
            //         //mapping mode is by polygon-vertex.
            //         //we can get normals by retrieving polygon-vertex.
            //        else if (normal_elem->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
            //        {
            //            int index_by_polygon_vertex = 0;
            //            //Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
            //            for (int polygon_index = 0; polygon_index < mesh->GetPolygonCount(); polygon_index++)
            //            {
            //                //get polygon size, you know how many vertices in current polygon.
            //                int polygon_size = mesh->GetPolygonSize(polygon_index);
            //                //retrieve each vertex of current polygon.
            //                for (int i = 0; i < polygon_size; i++)
            //                {
            //                    int normal_index = 0;
            //                    //reference mode is direct, the normal index is same as lIndexByPolygonVertex.
            //                    if (normal_elem->GetReferenceMode() == FbxGeometryElement::eDirect)
            //                        normal_index = index_by_polygon_vertex;

            //                    //reference mode is index-to-direct, get normals by the index-to-direct
            //                    if (normal_elem->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
            //                        normal_index = normal_elem->GetIndexArray().GetAt(index_by_polygon_vertex);

            //                    //Got normals of each polygon-vertex.
            //                    FbxVector4 normal = normal_elem->GetDirectArray().GetAt(normal_index);
            //                    if (gVerbose)
            //                    {
            //                        FBXSDK_printf("normals for polygon[%d]vertex[%d]: %f %f %f %f \n",
            //                            polygon_index, i, normal[0], normal[1], normal[2], normal[3]);
            //                    }
            //                    //add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
            //                    //. . .

            //                    index_by_polygon_vertex++;
            //                }//end for i //lPolygonSize
            //            }//end for lPolygonIndex //PolygonCount

            //        }//end eByPolygonVertex
            //    }//end if lNormalElement
            //}

            //for (int cp_index = 0, cp_count = mesh->GetControlPointsCount(); cp_index < cp_count; ++cp_index)
            //{
            //    fbxsdk::FbxVector4 cp = mesh->GetControlPointAt(cp_index);
            //    FBXSDK_printf("cp[%d]: %f %f %f %f \n",
            //        cp_index, cp[0], cp[1], cp[2], cp[3]);
            //}

            //for (int i = 0; i < polygonCount; ++i)
            //{
            //    indexList.push_back(mesh->GetPolygonVertex(i, 0));
            //    indexList.push_back(mesh->GetPolygonVertex(i, 1));
            //    indexList.push_back(mesh->GetPolygonVertex(i, 2));
            //}

            GetIndexList(&o_geom->draw_element_indices(), mesh);
            GetPositionList(&o_geom->vertex_positions(), mesh, o_geom->draw_element_indices());
            GetNormalList(&o_geom->vertex_normals(), mesh, o_geom->draw_element_indices());

        }//end if lMesh

         //recursively traverse each node in the scene
        int i, count = node->GetChildCount();
        for (i = 0; i < count; i++)
        {
            GetNormals(o_geom, node->GetChild(i));
        }
    }

    void DisplayHierarchy(FbxNode* pNode, int pDepth)
    {
        FbxString lString;
        int i;

        for (i = 0; i < pDepth; i++)
        {
            lString += "     ";
        }

        lString += pNode->GetName();
        lString += "\n";

        FBXSDK_printf(lString.Buffer());

        for (i = 0; i < pNode->GetChildCount(); i++)
        {
            DisplayHierarchy(pNode->GetChild(i), pDepth + 1);
        }
    }


    void DisplayPose(FbxScene* pScene)
    {
        int      i, j, k, lPoseCount;
        FbxString  lName;

        lPoseCount = pScene->GetPoseCount();

        for (i = 0; i < lPoseCount; i++)
        {
            FbxPose* lPose = pScene->GetPose(i);

            lName = lPose->GetName();
            printf("Pose Name: %s\n", lName.Buffer());

            printf("    Is a bind pose: %s\n", lPose->IsBindPose() ? "true" : "false");

            printf("    Number of items in the pose: %d\n", lPose->GetCount());

            printf("\n");

            for (j = 0; j<lPose->GetCount(); j++)
            {
                lName = lPose->GetNodeName(j).GetCurrentName();
                printf("    Item name: %s\n", lName.Buffer());

                if (!lPose->IsBindPose())
                {
                    // Rest pose can have local matrix
                    printf("    Is local space matrix: %s\n", lPose->IsLocalMatrix(j) ? "true" : "false");
                }

                printf("    Matrix value: \n");

                FbxString lMatrixValue;

                FbxNode* node = lPose->GetNode(j);

                for (k = 0; k<4; k++)
                {
                    FbxMatrix  lMatrix = lPose->GetMatrix(j);
                    FbxVector4 lRow = lMatrix.GetRow(k);
                    char        lRowValue[1024];

                    FBXSDK_sprintf(lRowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
                    lMatrixValue += FbxString("        ") + FbxString(lRowValue);
                }

                printf("%s\n", lMatrixValue.Buffer());
            }
        }

        lPoseCount = pScene->GetCharacterPoseCount();

        for (i = 0; i < lPoseCount; i++)
        {
            FbxCharacterPose* lPose = pScene->GetCharacterPose(i);
            FbxCharacter*     lCharacter = lPose->GetCharacter();

            if (!lCharacter) break;

            printf("Character Pose Name: %s\n", lCharacter->GetName());

            FbxCharacterLink lCharacterLink;
            FbxCharacter::ENodeId  lNodeId = FbxCharacter::eHips;

            while (lCharacter->GetCharacterLink(lNodeId, &lCharacterLink))
            {
                FbxAMatrix& lGlobalPosition = lCharacterLink.mNode->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);

                printf("    Matrix value: %s\n", "");

                FbxString lMatrixValue;

                for (k = 0; k<4; k++)
                {
                    FbxVector4 lRow = lGlobalPosition.GetRow(k);
                    char        lRowValue[1024];

                    FBXSDK_sprintf(lRowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
                    lMatrixValue += FbxString("        ") + FbxString(lRowValue);
                }

                printf("%s\n", lMatrixValue.Buffer());

                lNodeId = FbxCharacter::ENodeId(int(lNodeId) + 1);
            }
        }
    }

    void PrintLocalSrtAndTransformMatrixOfJoints(FbxNode* root_node)
    {
        FbxMesh* mesh = root_node->GetMesh();
        int skin_count = mesh->GetDeformerCount(FbxDeformer::eSkin);
        for (int skin_index = 0; skin_index < skin_count; ++skin_index)
        {
            FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(skin_index, FbxDeformer::eSkin));
            int cluster_count = skin->GetClusterCount();
            for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
            {
                FbxCluster* cluster = skin->GetCluster(cluster_index);
                FbxNode* linked_node = cluster->GetLink();
                printf("%s:\n", linked_node->GetName());

                // ジョイントのローカル SRT を表示
                fbxsdk::FbxDouble3 local_translate = linked_node->LclTranslation.Get();
                fbxsdk::FbxDouble3 local_rotate = linked_node->LclRotation.Get();
                fbxsdk::FbxDouble3 local_scale = linked_node->LclScaling.Get();
                printf("    t = (%8.3f, %8.3f, %8.3f)\n    r = (%8.3f, %8.3f, %8.3f)\n    s = (%8.3f, %8.3f, %8.3f)\n",
                    local_translate[0],
                    local_translate[1],
                    local_translate[2],
                    local_rotate[0],
                    local_rotate[1],
                    local_rotate[2],
                    local_scale[0],
                    local_scale[1],
                    local_scale[2]);

                // ジョイントバインドポーズの変換行列を表示
                fbxsdk::FbxAMatrix init_matrix;
                cluster->GetTransformLinkMatrix(init_matrix);
                printf("    matrix =\n");
                for (int row = 0; row < 4; ++row)
                {
                    printf("        ");
                    for (int col = 0; col < 4; ++col)
                    {
                        printf("%8.3f, ", init_matrix.Get(row, col));
                    }
                    printf("\n");
                }
            }
        }
    }

    void GetSkinningIndicesAndWeights(wsp::graphic::Obj* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, FbxNode* root_node)
    {
        PrintLocalSrtAndTransformMatrixOfJoints(root_node);

        FbxMesh* mesh = root_node->GetMesh();

        o_geom->num_skinning_weights = mesh->GetControlPointsCount();
        o_geom->skinning_weights.clear();
        o_geom->skinning_weights.reserve(o_geom->num_skinning_weights);
        o_geom->skinning_indices.clear();
        o_geom->skinning_indices.reserve(o_geom->num_skinning_weights);

        // 初期化
        for (int vertex_index = 0; vertex_index < o_geom->num_skinning_weights; ++vertex_index)
        {
            o_geom->skinning_indices.push_back(WspIntVec4(InvalidBoneIndex, InvalidBoneIndex, InvalidBoneIndex, InvalidBoneIndex));
            o_geom->skinning_weights.push_back(WspVec4F(0.0f, 0.0f, 0.0f, 0.0f));
        }

        int skin_count = mesh->GetDeformerCount(FbxDeformer::eSkin);
        for (int skin_index = 0; skin_index < skin_count; ++skin_index)
        {
            FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(skin_index, FbxDeformer::eSkin));
            int cluster_count = skin->GetClusterCount();

            for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
            {
                FbxCluster* cluster = skin->GetCluster(cluster_index);
                fbxsdk::FbxAMatrix init_matrix;
                cluster->GetTransformLinkMatrix(init_matrix);

                FbxNode* linked_node = cluster->GetLink();
                fbxsdk::FbxDouble3 local_translate = linked_node->LclTranslation.Get();
                fbxsdk::FbxDouble3 local_rotate = linked_node->LclRotation.Get();
                fbxsdk::FbxDouble3 local_scale = linked_node->LclScaling.Get();

                wsp::graphic::RenderBone* bone = o_skeleton->FindBone(linked_node->GetName());
                if (bone == nullptr)
                {
                    AddBone(o_skeleton, linked_node);
                    int bone_index = o_skeleton->GetBoneCount() - 1;
                    bone = o_skeleton->GetBone(bone_index);
                }

                int matrix_table_index = bone->GetSmoothSkinningMatrixTableIndex();

                int	vertex_index_count = cluster->GetControlPointIndicesCount();
                int* vertex_indices = cluster->GetControlPointIndices();
                double* vertex_weights = cluster->GetControlPointWeights();
                for (int vertex_array_index = 0; vertex_array_index < vertex_index_count; ++vertex_array_index)
                {
                    int vertex_index = vertex_indices[vertex_array_index];
                    float weight = vertex_weights[vertex_array_index];
                    if (weight > 0.0f && weight <= 1.0f)
                    {
                        float* weights = reinterpret_cast<float*>(&o_geom->skinning_weights[vertex_index]);
                        int* bone_indices = reinterpret_cast<int*>(&o_geom->skinning_indices[vertex_index]);
                        bool found = false;
                        for (int index = 0; index < 4; ++index)
                        {
                            if (bone_indices[index] == InvalidBoneIndex)
                            {
                                bone_indices[index] = matrix_table_index;
                                weights[index] = weight;
                                found = true;
                                break;
                            }
                        }

                        WSP_FATAL_IF_FALSE(found, "exceeds max vertex skinning count 4");
                    }
                }
            }
        }

        if (skin_count > 0)
        {
            for (int vertex_index = 0; vertex_index < o_geom->num_skinning_weights; ++vertex_index)
            {
                WspIntVec4& bone_indices = o_geom->skinning_indices[vertex_index];
                WspVec4F& bone_weights = o_geom->skinning_weights[vertex_index];
                if (bone_indices.x == InvalidBoneIndex)
                {
                    bone_indices.x = 0;
                    bone_weights.x = 0.0f;
                }
                if (bone_indices.y == InvalidBoneIndex)
                {
                    bone_indices.y = 0;
                    bone_weights.y = 0.0f;
                }
                if (bone_indices.z == InvalidBoneIndex)
                {
                    bone_indices.z = 0;
                    bone_weights.z = 0.0f;
                }
                if (bone_indices.w == InvalidBoneIndex)
                {
                    bone_indices.w = 0;
                    bone_weights.w = 0.0f;
                }
            }
        }

        o_skeleton->ResolveBoneLinks();

        o_geom->triangle_skinning_index_indices.clear();
        o_geom->triangle_skinning_index_indices.reserve(o_geom->num_triangles * 3);
        o_geom->triangle_skinning_weight_indices.clear();
        o_geom->triangle_skinning_weight_indices.reserve(o_geom->num_triangles * 3);
        for (int i = 0; i < o_geom->num_triangles; ++i)
        {
            int tri_vert0_index = mesh->GetPolygonVertex(i, 0);
            int tri_vert1_index = mesh->GetPolygonVertex(i, 1);
            int tri_vert2_index = mesh->GetPolygonVertex(i, 2);
            o_geom->triangle_skinning_index_indices.push_back(tri_vert0_index);
            o_geom->triangle_skinning_index_indices.push_back(tri_vert1_index);
            o_geom->triangle_skinning_index_indices.push_back(tri_vert2_index);

            o_geom->triangle_skinning_weight_indices.push_back(tri_vert0_index);
            o_geom->triangle_skinning_weight_indices.push_back(tri_vert1_index);
            o_geom->triangle_skinning_weight_indices.push_back(tri_vert2_index);
        }
    }

    void RetrieveMaterial(wsp::graphic::Mtl* o_mtl, FbxMesh* mesh)
    {
        // マテリアルのロード
        int material_count = mesh->GetNode()->GetSrcObjectCount(fbxsdk::FbxCriteria::ObjectType(FbxSurfaceMaterial::ClassId));
        WSP_ASSERT(material_count <= 1, "unsupported material count");
        for (int material_index = 0; material_index < material_count; ++material_index)
        {
            fbxsdk::FbxSurfaceMaterial* material = mesh->GetNode()->GetSrcObject<fbxsdk::FbxSurfaceMaterial>(material_index);
            if (material == nullptr)
            {
                continue;
            }

            wsp::graphic::Mtl mtl;
            strcpy(mtl.name, material->GetName());

            // 各マテリアルの処理
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)material;
                WSP_UNUSED(lambert);
                // 何か処理が必要であれば
            }
            else if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
            {
                FbxSurfacePhong* phong = (FbxSurfacePhong*)material;
                WSP_UNUSED(phong);
                // 何か処理が必要であれば
            }

            // テクスチャのロード
            FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            int layered_tex_count = property.GetSrcObjectCount<FbxLayeredTexture>();
            if (layered_tex_count > 0)
            {
                for (int layered_tex_index = 0; layered_tex_index < layered_tex_count; layered_tex_index++)
                {
                    FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>(property.GetSrcObject<FbxLayeredTexture>(layered_tex_index));
                    int tex_count = layered_texture->GetSrcObjectCount<FbxTexture>();

                    for (int tex_index = 0; tex_index < tex_count; ++tex_index)
                    {
                        FbxFileTexture* texture = layered_texture->GetSrcObject<FbxFileTexture>(tex_index);
                        const char* tex_name = texture->GetName();
                        printf("layered texture %s: %s\n", tex_name, texture->GetFileName());
                    }
                }
            }
            else
            {
                int tex_count = property.GetSrcObjectCount(fbxsdk::FbxCriteria::ObjectType(FbxTexture::ClassId));
                WSP_ASSERT(tex_count <= 1, "unsupported texture count");

                for (int tex_index = 0; tex_index < tex_count; ++tex_index)
                {
                    FbxFileTexture* texture = property.GetSrcObject<FbxFileTexture>(tex_index);
                    const char* tex_name = texture->GetName();
                    const char* tex_file_abs_path = texture->GetFileName();
                    WSP_UNUSED(tex_file_abs_path);
                    const char* tex_file_name = texture->GetRelativeFileName();
                    WSP_GetFileNameFromFilePath(mtl.texture_name, texture->GetRelativeFileName());
                    printf("texture %s: path=%s\n", tex_name, mtl.texture_name);
                }
            }

            *o_mtl = mtl;
        }
    }

    void RetrieveNodeInfo(wsp::graphic::Obj* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, FbxNode* node)
    {
        if (!node)
        {
            return;
        }

        //get mesh
        FbxMesh* mesh = node->GetMesh();
        if (mesh)
        {
            GetPositionList(o_geom, mesh);
            GetNormalList(o_geom, mesh);
            GetTextureCoordinates(o_geom, mesh);
            if (o_skeleton != nullptr)
            {
                GetSkinningIndicesAndWeights(o_geom, o_skeleton, node);
            }
        }

        //recursively traverse each node in the scene
        for (int i = 0, count = node->GetChildCount(); i < count; ++i)
        {
            RetrieveNodeInfo(o_geom, o_skeleton, node->GetChild(i));
        }
    }


    void RetrieveNodeInfo(
        wsp::Vector<wsp::graphic::Obj*>* o_geoms,
        wsp::graphic::RenderSkeleton* o_skeleton,
        FbxNode* node)
    {
        if (!node)
        {
            return;
        }

        printf("Node Name: %s\n", node->GetName());

        //get mesh
        FbxMesh* mesh = node->GetMesh();
        if (mesh)
        {
            wsp::graphic::Obj* geom = WSP_NEW wsp::graphic::Obj();
            GetPositionList(geom, mesh);
            GetNormalList(geom, mesh);
            GetTextureCoordinates(geom, mesh);
            if (o_skeleton != nullptr)
            {
                GetSkinningIndicesAndWeights(geom, o_skeleton, node);
            }

            wsp::graphic::RenderBone* parent_bone = o_skeleton->FindBone(node->GetParent()->GetName());
            WSP_ASSERT_NOT_NULL(parent_bone);

            geom->parent_bone_index = parent_bone->GetSmoothSkinningMatrixTableIndex();
            o_geoms->Append(geom);

            RetrieveMaterial(&geom->material, mesh);
        }
        else
        {
            wsp::graphic::RenderBone* bone = o_skeleton->FindBone(node->GetName());
            if (bone == nullptr)
            {
                AddBone(o_skeleton, node);
            }
        }

        //recursively traverse each node in the scene
        for (int i = 0, count = node->GetChildCount(); i < count; ++i)
        {
            RetrieveNodeInfo(o_geoms, o_skeleton, node->GetChild(i));
        }
    }

    class ScopedFbxSdkInitializer
    {
    public:
        ScopedFbxSdkInitializer()
        {
            // Prepare the FBX SDK.
            //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
            sdk_manager_ = FbxManager::Create();
            if (!sdk_manager_)
            {
                FBXSDK_printf("Error: Unable to create FBX Manager!\n");
                exit(1);
            }
            else FBXSDK_printf("Autodesk FBX SDK version %s\n", sdk_manager_->GetVersion());

            //Create an IOSettings object. This object holds all import/export settings.
            FbxIOSettings* ios = FbxIOSettings::Create(sdk_manager_, IOSROOT);
            sdk_manager_->SetIOSettings(ios);

            //Load plugins from the executable directory (optional)
            FbxString app_dir_path = FbxGetApplicationDirectory();
            sdk_manager_->LoadPluginsDirectory(app_dir_path.Buffer());

            //Create an FBX scene. This object holds most objects imported/exported from/to files.
            scene_ = FbxScene::Create(sdk_manager_, "My Scene");
            if (!scene_)
            {
                FBXSDK_printf("Error: Unable to create FBX scene!\n");
                exit(1);
            }
        }

        ~ScopedFbxSdkInitializer()
        {
            sdk_manager_->Destroy();
        }

        FbxManager* GetSdkManager()
        {
            return sdk_manager_;
        }

        FbxScene* GetScene()
        {
            return scene_;
        }

    private:
        FbxManager * sdk_manager_ = NULL;
        FbxScene* scene_ = NULL;
    };
}

// ==============================================================================
// ==============================================================================
// ==============================================================================
// ==============================================================================


wsp::fbx::FbxResult wsp::fbx::LoadFbx(wsp::graphic::RenderGeometry* o_geom, const char* file_path)
{
    wsp::graphic::Obj obj_data;
    wsp::fbx::FbxResult result = wsp::fbx::LoadFbx(&obj_data, file_path);
    if (result != wsp::fbx::FBX_RESULT_NO_ERROR)
    {
        return result;
    }

    if (obj_data.num_normals == 0)
    {
        obj_data.num_normals = obj_data.num_vertices;
        obj_data.normals.resize(obj_data.num_vertices);
        wsp::graphic::CreateNormalsFromVertices(
            (wsp::Vector3<float>*)obj_data.normals.data(), obj_data.num_normals,
            (wsp::Vector3<float>*)obj_data.vertex_positions.data(), obj_data.num_vertices,
            obj_data.triangle_vertex_position_indices.data(), obj_data.num_triangles * 3);

        obj_data.triangle_normal_indices.resize(obj_data.triangle_vertex_position_indices.size());
        memcpy(obj_data.triangle_normal_indices.data(), obj_data.triangle_vertex_position_indices.data(), sizeof(int) * obj_data.triangle_vertex_position_indices.size());
    }

    wsp::graphic::ConvertObjToIndexedElementData(
        o_geom->vertex_positions(),
        o_geom->vertex_texture_coords(),
        o_geom->vertex_normals(),
        o_geom->draw_element_indices(),
        obj_data);

    return wsp::fbx::FBX_RESULT_NO_ERROR;
}

wsp::fbx::FbxResult wsp::fbx::LoadFbx(
    wsp::graphic::RenderGeometry* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, const char* file_path)
{
    return wsp::fbx::LoadFbx(o_geom, o_skeleton, nullptr, file_path);
}


wsp::fbx::FbxResult wsp::fbx::LoadFbx(
	wsp::graphic::RenderGeometry* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, wsp::graphic::SkeletalAnim* o_skeletal_anim, const char* file_path)
{
	wsp::graphic::Obj obj_data;
	wsp::fbx::FbxResult result = wsp::fbx::LoadFbx(&obj_data, o_skeleton, o_skeletal_anim, file_path);
	if (result != wsp::fbx::FBX_RESULT_NO_ERROR)
	{
		return result;
	}

	// 法線の作成
	if (obj_data.num_normals == 0)
	{
		obj_data.num_normals = obj_data.num_vertices;
		obj_data.normals.resize(obj_data.num_vertices);
		wsp::graphic::CreateNormalsFromVertices(
			(wsp::Vector3<float>*)obj_data.normals.data(), obj_data.num_normals,
			(wsp::Vector3<float>*)obj_data.vertex_positions.data(), obj_data.num_vertices,
			obj_data.triangle_vertex_position_indices.data(), obj_data.num_triangles * 3);

		obj_data.triangle_normal_indices.resize(obj_data.triangle_vertex_position_indices.size());
		memcpy(obj_data.triangle_normal_indices.data(), obj_data.triangle_vertex_position_indices.data(), sizeof(int) * obj_data.triangle_vertex_position_indices.size());
	}

	wsp::graphic::ConvertObjToIndexedElementData(
		o_geom->vertex_positions(),
		o_geom->vertex_texture_coords(),
		o_geom->vertex_normals(),
		o_geom->vertex_skinning_bone_indices(),
		o_geom->vertex_skinning_weights(),
		o_geom->draw_element_indices(),
		obj_data);

	// 位置座標をスムーススキニング用に変換
	if (o_geom->IsSkinned())
	{
        o_skeleton->CalculateAndStoreInverseBindPoseMatrices();

        o_geom->skinned_vertex_positions().SetLength(o_geom->vertex_positions().length());
        o_geom->skinned_vertex_normals().SetLength(o_geom->vertex_normals().length());
        memcpy(o_geom->skinned_vertex_positions().data(), o_geom->vertex_positions().data(), o_geom->vertex_positions().length() * sizeof(WspVec3F));
        memcpy(o_geom->skinned_vertex_normals().data(), o_geom->vertex_normals().data(), o_geom->vertex_normals().length() * sizeof(WspVec3F));


 //       o_skeleton->UpdateMatrixPalette();
	//	wsp::graphic::ConvertToSmoothSkinnedPositionsAndNormals(
	//		&o_geom->skinned_vertex_positions(),
	//		&o_geom->skinned_vertex_normals(),
	//		o_geom->vertex_positions(),
	//		o_geom->vertex_normals(),
	//		o_geom->vertex_skinning_bone_indices(),
	//		o_geom->vertex_skinning_weights(),
	//		*o_skeleton);
	}

	return wsp::fbx::FBX_RESULT_NO_ERROR;
}



wsp::fbx::FbxResult wsp::fbx::LoadFbx(
    wsp::Vector<wsp::graphic::RenderGeometry*>* o_geoms, wsp::graphic::RenderSkeleton* o_skeleton, wsp::graphic::SkeletalAnim* o_skeletal_anim, const char* file_path)
{
    ScopedPtrVector<wsp::graphic::Obj> temp_objs;
    wsp::fbx::FbxResult result = wsp::fbx::LoadFbx(temp_objs.vector(), o_skeleton, o_skeletal_anim, file_path);
    if (result != wsp::fbx::FBX_RESULT_NO_ERROR)
    {
        return result;
    }

    // シェイプがないモデルはとりあえず許容しないでおく
    WSP_FATAL_IF_FALSE(temp_objs.vector()->length() > 0, "no shapes in %s", file_path);

    o_skeleton->CalculateAndStoreInverseBindPoseMatrices();

    for (int shape_index = 0; shape_index < temp_objs.vector()->length(); ++shape_index)
    {
        wsp::graphic::Obj* obj_data = (*temp_objs.vector())[shape_index];
        if (obj_data->num_normals == 0)
        {
            // 法線の作成
            obj_data->num_normals = obj_data->num_vertices;
            obj_data->normals.resize(obj_data->num_vertices);
            wsp::graphic::CreateNormalsFromVertices(
                (wsp::Vector3<float>*)obj_data->normals.data(), obj_data->num_normals,
                (wsp::Vector3<float>*)obj_data->vertex_positions.data(), obj_data->num_vertices,
                obj_data->triangle_vertex_position_indices.data(), obj_data->num_triangles * 3);

            obj_data->triangle_normal_indices.resize(obj_data->triangle_vertex_position_indices.size());
            memcpy(obj_data->triangle_normal_indices.data(), obj_data->triangle_vertex_position_indices.data(), sizeof(int) * obj_data->triangle_vertex_position_indices.size());
        }

        wsp::graphic::RenderGeometry* geom = WSP_NEW wsp::graphic::RenderGeometry();
        geom->set_parent_bone_index(obj_data->parent_bone_index);

        wsp::graphic::Material material;
        material.SetName(obj_data->material.name);
        material.SetDiffuseTextuerFileName(obj_data->material.texture_name);
        geom->set_material(material);

        wsp::graphic::ConvertObjToIndexedElementData(
            geom->vertex_positions(),
            geom->vertex_texture_coords(),
            geom->vertex_normals(),
            geom->vertex_skinning_bone_indices(),
            geom->vertex_skinning_weights(),
            geom->draw_element_indices(),
            *obj_data);

        // 位置座標をスムーススキニング用に変換
        if (geom->IsSkinned())
        {
            geom->skinned_vertex_positions().SetLength(geom->vertex_positions().length());
            geom->skinned_vertex_normals().SetLength(geom->vertex_normals().length());
            memcpy(geom->skinned_vertex_positions().data(), geom->vertex_positions().data(), geom->vertex_positions().length() * sizeof(WspVec3F));
            memcpy(geom->skinned_vertex_normals().data(), geom->vertex_normals().data(), geom->vertex_normals().length() * sizeof(WspVec3F));

        //    wsp::graphic::ConvertToSmoothSkinnedPositionsAndNormals(
        //        &geom->skinned_vertex_positions(),
        //        &geom->skinned_vertex_normals(),
        //        geom->vertex_positions(),
        //        geom->vertex_normals(),
        //        geom->vertex_skinning_bone_indices(),
        //        geom->vertex_skinning_weights(),
        //        *o_skeleton);
        }

        o_geoms->Append(geom);
    }

    return wsp::fbx::FBX_RESULT_NO_ERROR;
}

wsp::fbx::FbxResult wsp::fbx::LoadFbx(wsp::graphic::Obj* o_geom, const char* file_path)
{
    return LoadFbx(o_geom, nullptr, file_path);
}

wsp::fbx::FbxResult wsp::fbx::LoadFbx(wsp::graphic::Obj* o_geom, wsp::graphic::RenderSkeleton* o_skeleton, const char* file_path)
{
    return wsp::fbx::LoadFbx(o_geom, o_skeleton, nullptr, file_path);
}

bool IsSkinned(wsp::graphic::Obj* o_geom)
{
	return o_geom->skinning_indices[0].x == InvalidBoneIndex;
}

wsp::fbx::FbxResult wsp::fbx::LoadFbx(
	wsp::graphic::Obj* o_geom,
	wsp::graphic::RenderSkeleton* o_skeleton,
	wsp::graphic::SkeletalAnim* o_skeletal_anim,
	const char* file_path)
{
	ScopedFbxSdkInitializer fbx_sdk_initializer;
	bool success;

	success = LoadScene(fbx_sdk_initializer.GetSdkManager(), fbx_sdk_initializer.GetScene(), file_path);
	if (success == false)
	{
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
		return wsp::fbx::FBX_RESULT_FAILED_TO_IMPORT_SCENE;
	}

	FbxScene* scene = fbx_sdk_initializer.GetScene();
	DisplayPose(scene);
	FbxNode* root_node = scene->GetRootNode();
	DisplayHierarchy(root_node, 0);

    AddBones(o_skeleton, root_node);

	RetrieveNodeInfo(o_geom, o_skeleton, root_node);

	if (o_skeletal_anim != nullptr)
	{
		// アニメーション取得
		DisplayAnimation(o_skeletal_anim, scene);
	}

	// スキニングされていない場合は除外
	bool isSkinned = o_geom->skinning_indices[0].x != InvalidBoneIndex;
	if (!isSkinned)
	{
		o_geom->skinning_indices.resize(0);
		o_geom->skinning_weights.resize(0);
		o_geom->triangle_skinning_index_indices.resize(0);
		o_geom->triangle_skinning_weight_indices.resize(0);
		o_geom->num_skinning_weights = 0;
	}

	return wsp::fbx::FBX_RESULT_NO_ERROR;
}

wsp::fbx::FbxResult wsp::fbx::LoadFbx(
    wsp::Vector<wsp::graphic::Obj*>* o_geoms, wsp::graphic::RenderSkeleton* o_skeleton, wsp::graphic::SkeletalAnim* o_skeletal_anim, const char* file_path)
{
    ScopedFbxSdkInitializer fbx_sdk_initializer;
    bool success;

    success = LoadScene(fbx_sdk_initializer.GetSdkManager(), fbx_sdk_initializer.GetScene(), file_path);
    if (success == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene...");
        return wsp::fbx::FBX_RESULT_FAILED_TO_IMPORT_SCENE;
    }

    FbxScene* scene = fbx_sdk_initializer.GetScene();
    DisplayPose(scene);
    FbxNode* root_node = scene->GetRootNode();
    DisplayHierarchy(root_node, 0);

    AddBones(o_skeleton, root_node);
    RetrieveNodeInfo(o_geoms, o_skeleton, root_node);

    if (o_skeletal_anim != nullptr)
    {
        // アニメーション取得
        DisplayAnimation(o_skeletal_anim, scene);
    }

    // スキニングされていない場合は除外
    for (int i = 0; i < o_geoms->length(); ++i)
    {
        wsp::graphic::Obj* geom = (*o_geoms)[i];
        bool isSkinned = geom->skinning_indices[0].x != InvalidBoneIndex;
        if (!isSkinned)
        {
            geom->skinning_indices.resize(0);
            geom->skinning_weights.resize(0);
            geom->triangle_skinning_index_indices.resize(0);
            geom->triangle_skinning_weight_indices.resize(0);
            geom->num_skinning_weights = 0;
        }
    }

    return wsp::fbx::FBX_RESULT_NO_ERROR;
}