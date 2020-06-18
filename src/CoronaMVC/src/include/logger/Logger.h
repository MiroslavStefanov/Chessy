#pragma once
#include "logger/LogSeverity.h"
#include "logger/LoggerImpl.h"

#define LogReturnIf(condition, returnExpression, ...) \
if(condition) \
{ \
	mvc::Logger::Get().LogError(#condition, __VA_ARGS__); \
	DEBUG_BREAK_IF(true); \
	return returnExpression; \
}

#define VOID_RETURN 

#define LogContinueIf(condition, ...) \
if(condition) \
{ \
	mvc::Logger::Get().LogError(#condition, __VA_ARGS__); \
	DEBUG_BREAK_IF(true); \
	continue; \
}

#define LogBreakIf(condition, ...) \
if(condition) \
{ \
	mvc::Logger::Get().LogError(#condition, __VA_ARGS__); \
	DEBUG_BREAK_IF(true); \
	break; \
}

namespace mvc
{
	class Logger
	{
	private:
		struct Placeholder
		{
			static constexpr auto START_SYMBOL = '{';
			static constexpr auto END_SYMBOL = '}';

			Placeholder(std::size_t start, std::size_t length);
			std::string GetReplacement(const std::string& message, const std::vector<std::string>& replacements) const;
			std::size_t GetStartPosition() const;
			std::size_t GetEndPosition() const;

		private:
			std::size_t m_startPosition;
			std::size_t m_length;
		};

	public:
		static Logger& Get();

	public:
		template<class ...Ts>
		void LogMessage(std::string message, const Ts& ...params);

		template<class ...Ts>
		void LogWarning(std::string message, const Ts& ...params);

		template<class ...Ts>
		void LogError(std::string message, const Ts& ...params);

		void SetLoggerImplementation(std::unique_ptr<LoggerImpl>&& implementation);

	private:
		template<class ...Ts>
		void PrintLog(const std::string& message, ELogSeverity severity, const Ts& ...params);

		std::vector<Placeholder> CollectPlaceholders(const std::string& message) const;
		std::string ReplaceValues(const std::string& message, const std::vector<Placeholder>& placeholders, const std::vector<std::string>& replacements) const;

		template<class ...Ts>
		std::vector<std::string> StringifyParams(const Ts& ...params) const;

	private:
		std::unique_ptr<LoggerImpl> m_impl;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class ...Ts>
	void Logger::LogMessage(std::string message, const Ts& ...params)
	{
		PrintLog(message, ELogSeverity::Message, params...);
	}

	template<class ...Ts>
	////////////////////////////////////////////////////////////////////////////////////////////////
	void Logger::LogWarning(std::string message, const Ts& ...params)
	{
		PrintLog(message, ELogSeverity::Warning, params...);
	}

	template<class ...Ts>
	////////////////////////////////////////////////////////////////////////////////////////////////
	void Logger::LogError(std::string message, const Ts& ...params)
	{
		PrintLog(message, ELogSeverity::Error, params...);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class ...Ts>
	void Logger::PrintLog(const std::string& message, ELogSeverity severity, const Ts& ...params)
	{
		auto placeholders = CollectPlaceholders(message);
		auto replacements = StringifyParams(params...);
		auto replacedMessage = ReplaceValues(message, placeholders, replacements);

		assert(m_impl);
		m_impl->PrintLog(replacedMessage, severity);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class ...Ts>
	std::vector<std::string> Logger::StringifyParams(const Ts& ...params) const
	{
		std::stringstream stream;
		((stream << params << ' '), ...);

		std::vector<std::string> result;
		while (!stream.eof())
		{
			std::string paramString;
			stream >> paramString;
			if (!paramString.empty())
			{
				result.push_back(std::move(paramString));
			}
		}

		return result;
	}
}