#pragma once

#include <wsp/common/common_types.h>
#include <wsp/common/_define_commoncore.h>

#include <map>
#include <string>

namespace wsp{
    namespace xerces{
        class WSP_DLL_EXPORT XmlHandler
        {
        public:
            XmlHandler();
            ~XmlHandler();

            // Getter ------------------------
            bool HasParameter(const char* param_name) const;
            int GetIntParameter(const std::string &param_name) const;
            std::string GetStringParameter(const std::string &param_name) const;
            float GetFloatParameter(const std::string &paramName) const;
            bool GetBoolParameter(const std::string &paramName) const;

            // I/O ---------------------------
            wsp::State LoadParametersFromXml(const char* in_xml_path);
            wsp::State SaveParametersAsXml(const char* in_xml_path);

            void AddIntParameter(const std::string &param_name, int value);
            void AddStringParameter(const std::string &param_name, const std::string &value);
            void AddBoolParameter(const std::string &param_name, bool value);
            void AddFloatParamter(const std::string &param_name, float value);

        private:
            XmlHandler(XmlHandler &rhs);
            XmlHandler& operator = (XmlHandler &rhs);

            std::map<std::string, int> param_list_;
            std::map<std::string, float> float_param_list_;
            std::map<std::string, bool> bool_param_list_;
            std::map<std::string, std::string> str_param_list_;
        };
    }
}

