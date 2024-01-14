#pragma once

#include <ctime>
#include <chrono>

namespace wsp { 
	namespace common {
		class Stopwatch
		{
		public:
			Stopwatch()
			{
				start_time_ = std::chrono::high_resolution_clock::now();
			}

			void Restart()
			{
				start_time_ = std::chrono::high_resolution_clock::now();
			}

			double GetElaspedMilliseconds()
			{
				return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start_time_).count();
			}

		private:
			std::chrono::high_resolution_clock::time_point start_time_;
		};

        class ScopedElaspedMillisecondsLogger
        {
        public:
            explicit ScopedElaspedMillisecondsLogger(const char* log_label)
                : log_label_(log_label)
            {
            }

            ~ScopedElaspedMillisecondsLogger()
            {
                WSP_Printf("%s: %f ms\n", log_label_.c_str(), stopwatch_.GetElaspedMilliseconds());
            }

        private:
            wsp::common::Stopwatch stopwatch_;
            std::string log_label_;
        };
	}
}
