#pragma once

#include <wsp/common/tpl_cl-vector.h>

namespace wsp {
	namespace graphic {
		template<typename TValue>
		class AnimKey
		{
		public:
			AnimKey()
				: value_(static_cast<TValue>(0))
				, frame_(0.0f)
			{
			}

			AnimKey(TValue value, float frame)
				: value_(value)
				, frame_(frame)
			{
			}

			AnimKey(const AnimKey<TValue>& rhs)
			{
				value_ = rhs.value_;
				frame_ = rhs.frame_;
			}

			~AnimKey()
			{
			}

			TValue value() const
			{
				return value_;
			}

			float frame() const
			{
				return frame_;
			}

			void set_value(TValue value)
			{
				value_ = value;
			}

			void set_frame(float frame)
			{
				frame_ = frame;
			}

		private:
			TValue value_;
			float frame_;
		};

		template<typename TValue>
		class AnimCurve
		{
		public:
			AnimCurve()
				: base_value_(static_cast<TValue>(0))
				, min_frame_(FLT_MAX)
				, max_frame_(0.0f)
			{
			}

			AnimCurve(const AnimCurve<TValue>& rhs)
				: base_value_(rhs.base_value_)
			{
				keys_.Clear();
				for (int key_index = 0; key_index < keys_.length(); ++key_index)
				{
					const AnimKey<TValue>* key = &rhs.keys_[key_index];
					AddKey(key->value(), key->frame());
				}
			}

			~AnimCurve()
			{
			}

			void set_base_value(TValue base_value)
			{
				base_value_ = base_value;
			}

			TValue GetValue(float frame) const
			{
				if (keys_.length() == 0)
				{
					return base_value_;
				}

				const AnimKey<TValue>* prev_key = &keys_[0];
				if (frame < min_frame_)
				{
					return prev_key->value();
				}

                if (frame >= max_frame_)
                {
                    const AnimKey<TValue>* last_key = &keys_[keys_.length() - 1];
                    return last_key->value();
                }
				
				for (int key_index = 1; key_index < keys_.length(); ++key_index)
				{
					const AnimKey<TValue>* next_key = &keys_[key_index];

					// TODO: üŒ`•âŠÔ‚µ‚Ä‚¢‚é‚Ì‚ÅA‚¿‚á‚ñ‚Æ•âŠÔ‚·‚é
					if (prev_key->frame() <= frame && frame <= next_key->frame())
					{
						float next_key_ratio = (frame - prev_key->frame()) / (next_key->frame() - prev_key->frame());
						return next_key->value() * next_key_ratio + prev_key->value() * (1.0f - next_key_ratio);
					}

					prev_key = next_key;
				}

				return base_value_;
			}

			void AddKey(TValue value, float frame)
			{
				AnimKey<TValue> key(value, frame);
				keys_.Append(key);

				if (frame < min_frame_)
				{
					min_frame_ = frame;
				}
				else if (frame > max_frame_)
				{
					max_frame_ = frame;
				}
			}

			float min_frame() const
			{
				return min_frame_;
			}

			float max_frame() const
			{
				return max_frame_;
			}

			bool HasKey() const
			{
				return keys_.length() > 0;
			}

		private:
			wsp::Vector<AnimKey<TValue>> keys_;
			TValue base_value_;
			float min_frame_;
			float max_frame_;
		};

		class BoneAnim
		{
		public:
			inline BoneAnim()
			{
				tx_curve_.set_base_value(0.0f);
				ty_curve_.set_base_value(0.0f);
				tz_curve_.set_base_value(0.0f);
				rx_curve_.set_base_value(0.0f);
				ry_curve_.set_base_value(0.0f);
				rz_curve_.set_base_value(0.0f);
				sx_curve_.set_base_value(1.0f);
				sy_curve_.set_base_value(1.0f);
				sz_curve_.set_base_value(1.0f);
				curves_[0] = &tx_curve_;
				curves_[1] = &ty_curve_;
				curves_[2] = &tz_curve_;
				curves_[3] = &rx_curve_;
				curves_[4] = &ry_curve_;
				curves_[5] = &rz_curve_;
				curves_[6] = &sx_curve_;
				curves_[7] = &sy_curve_;
				curves_[8] = &sz_curve_;
			}

			inline BoneAnim(const BoneAnim& rhs)
				: tx_curve_(rhs.tx_curve_)
				, ty_curve_(rhs.ty_curve_)
				, tz_curve_(rhs.tz_curve_)
				, rx_curve_(rhs.rx_curve_)
				, ry_curve_(rhs.ry_curve_)
				, rz_curve_(rhs.rz_curve_)
				, sx_curve_(rhs.sx_curve_)
				, sy_curve_(rhs.sy_curve_)
				, sz_curve_(rhs.sz_curve_)
			{
				curves_[0] = &tx_curve_;
				curves_[1] = &ty_curve_;
				curves_[2] = &tz_curve_;
				curves_[3] = &rx_curve_;
				curves_[4] = &ry_curve_;
				curves_[5] = &rz_curve_;
				curves_[6] = &sx_curve_;
				curves_[7] = &sy_curve_;
				curves_[8] = &sz_curve_;
			}

			inline ~BoneAnim()
			{
			}

			inline const char* bone_name() const { return bone_name_; }
			inline void set_bone_name(const char* name);
			inline AnimCurve<float>& translate_x_curve() { return tx_curve_; }
			inline AnimCurve<float>& translate_y_curve() { return ty_curve_; }
			inline AnimCurve<float>& translate_z_curve() { return tz_curve_; }
			inline AnimCurve<float>& rotate_x_curve() { return rx_curve_; }
			inline AnimCurve<float>& rotate_y_curve() { return ry_curve_; }
			inline AnimCurve<float>& rotate_z_curve() { return rz_curve_; }
			inline AnimCurve<float>& scale_x_curve() { return sx_curve_; }
			inline AnimCurve<float>& scale_y_curve() { return sy_curve_; }
			inline AnimCurve<float>& scale_z_curve() { return sz_curve_; }

			inline const AnimCurve<float>& translate_x_curve() const { return tx_curve_; }
			inline const AnimCurve<float>& translate_y_curve() const { return ty_curve_; }
			inline const AnimCurve<float>& translate_z_curve() const { return tz_curve_; }
			inline const AnimCurve<float>& rotate_x_curve() const { return rx_curve_; }
			inline const AnimCurve<float>& rotate_y_curve() const { return ry_curve_; }
			inline const AnimCurve<float>& rotate_z_curve() const { return rz_curve_; }
			inline const AnimCurve<float>& scale_x_curve() const { return sx_curve_; }
			inline const AnimCurve<float>& scale_y_curve() const { return sy_curve_; }
			inline const AnimCurve<float>& scale_z_curve() const { return sz_curve_; }

			inline float FindMaxFrame() const
			{
				float max_frame = 0.0f;
				for (int i = 0; i < 9; ++i)
				{
					AnimCurve<float>* curve = curves_[i];
					if (curve->max_frame() > max_frame)
					{
						max_frame = curve->max_frame();
					}
				}

				return max_frame;
			}

			inline float FindMinFrame() const
			{
				float min_frame = FLT_MAX;
				for (int i = 0; i < 9; ++i)
				{
					AnimCurve<float>* curve = curves_[i];
					if (curve->min_frame() > min_frame)
					{
						min_frame = curve->min_frame();
					}
				}

				return min_frame;
			}

		private:
			static const size_t MaxBoneNameSize = 32;
			char bone_name_[MaxBoneNameSize];
			AnimCurve<float> tx_curve_;
			AnimCurve<float> ty_curve_;
			AnimCurve<float> tz_curve_;
			AnimCurve<float> rx_curve_;
			AnimCurve<float> ry_curve_;
			AnimCurve<float> rz_curve_;
			AnimCurve<float> sx_curve_;
			AnimCurve<float> sy_curve_;
			AnimCurve<float> sz_curve_;
			AnimCurve<float>* curves_[9];
		};

		void BoneAnim::set_bone_name(const char* name)
		{
			strncpy(bone_name_, name, MaxBoneNameSize);
		}

		class SkeletalAnim
		{
		public:
			inline SkeletalAnim()
				: is_loop_anim_(true)
			{
			}

			inline SkeletalAnim(const SkeletalAnim& rhs)
			{
				bone_anims_.Clear();
				for (int bone_anim_index = 0; bone_anim_index < bone_anims_.length(); ++bone_anim_index)
				{
					const BoneAnim* bone_anim = &rhs.bone_anims_[bone_anim_index];
					AddBoneAnim(*bone_anim);
				}

			}

			inline ~SkeletalAnim()
			{
			}

			bool is_loop_anim() const
			{
				return is_loop_anim_;
			}

			void SetLoopAnimEnabled()
			{
				is_loop_anim_ = true;
			}

			void SetLoopAnimDisabled()
			{
				is_loop_anim_ = false;
			}

			inline BoneAnim* GetBoneAnim(int index)
			{
				return &bone_anims_[index];
			}

			inline const BoneAnim* GetBoneAnim(int index) const
			{
				return &bone_anims_[index];
			}

			inline BoneAnim* GetBoneAnim(const char* bone_name)
			{
				for (int bone_index = 0, bone_count = bone_anims_.length(); bone_index < bone_count; ++bone_index)
				{
					wsp::graphic::BoneAnim& bone_anim = bone_anims_[bone_index];
					if (strcmp(bone_anim.bone_name(), bone_name) == 0)
					{
						return &bone_anim;
					}
				}

				return nullptr;
			}

			inline const BoneAnim* GetBoneAnim(const char* bone_name) const
			{
				for (int bone_index = 0, bone_count = bone_anims_.length(); bone_index < bone_count; ++bone_index)
				{
					const wsp::graphic::BoneAnim& bone_anim = bone_anims_[bone_index];
					if (strcmp(bone_anim.bone_name(), bone_name) == 0)
					{
						return &bone_anim;
					}
				}

				return nullptr;
			}

			inline void AddBoneAnim(const BoneAnim& bone_anim)
			{
				bone_anims_.Append(bone_anim);
				float max_frame = bone_anim.FindMaxFrame();
				if (max_frame > max_frame_)
				{
					max_frame_ = max_frame;
				}
			}

			inline float max_frame() const
			{
				return max_frame_;
			}

		private:
			wsp::Vector<BoneAnim> bone_anims_;
			bool is_loop_anim_;
			float max_frame_;
		};
	}
}