#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Tools.h"

namespace rww2hook
{
	constexpr char kLogFileName[] = "log.txt";

	class Log
	{
	public:
		static Log& get()
		{
			static Log instance;
			return instance;
		}

		template <typename... Args>
		void Write(Args&&... args)
		{
			std::stringstream str;
			str << tools::GetTime() << ": ";

			Output(str, std::forward<Args>(args)...);

			const auto text = str.str();

			try
			{
				ofstream_ << text;
				ofstream_.flush();
			}
			catch (const std::ofstream::failure& ex)
			{
				std::cerr << "Exception occured when writing to a file " << kLogFileName << ": " << ex.what() << std::endl;
			}

			std::cout << text;
		}

	private:
		Log() : ofstream_(kLogFileName)
		{
			if (!ofstream_.good())
				throw std::runtime_error("File open failure");

			ofstream_.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		}

		Log(const Log&) = delete;
		Log(Log&&) = delete;
		Log& operator=(const Log&) = delete;
		Log& operator=(Log&&) = delete;

		~Log()
		{
			ofstream_.close();
		}

		template <typename Arg, typename... Args>
		std::ostream& Output(std::ostream& o, Arg&& arg, Args&&... args) const
		{
			o << std::forward<Arg>(arg) << ' ';

			if (sizeof...(args))
				return Output(o, std::forward<Args>(args)...);

			return o << std::endl;
		}

		static std::ostream& Output(std::ostream& o)
		{
			return o << std::endl;
		}

		std::ofstream ofstream_;
	};
}
