#include "pch.h"
#include "Logger.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	Logger& Logger::Get()
	{
		static Logger instance;
		return instance;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void Logger::SetLoggerImplementation(std::unique_ptr<LoggerImpl>&& implementation)
	{
		m_impl = std::move(implementation);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<Logger::Placeholder> Logger::CollectPlaceholders(const std::string& message) const
	{
		std::vector<Placeholder> result;

		std::size_t lastEndPosition = 0;
		while (lastEndPosition < message.size())
		{
			auto placeholderStart = message.find(Placeholder::START_SYMBOL, lastEndPosition);
			if (placeholderStart == std::string::npos)
			{
				break;
			}

			auto placeholderEnd = message.find(Placeholder::END_SYMBOL, placeholderStart);
			if (placeholderEnd == std::string::npos)
			{
				lastEndPosition = placeholderStart + 1;
				LogContinueIf(true && "Invalid log message syntax. Not closed placeholder starting at position {0} in message {1}", placeholderStart, message);
			}

			result.emplace_back(placeholderStart + 1, placeholderEnd - placeholderStart - 1);
			lastEndPosition = placeholderEnd + 1;
		}

		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::string Logger::ReplaceValues(const std::string& message, const std::vector<Placeholder>& placeholders, const std::vector<std::string>& replacements) const
	{
		std::stringstream stream;

		std::size_t startPosition = 0;
		for (const auto& placeholder : placeholders)
		{
			auto beforePlaceholderString = message.substr(startPosition, placeholder.GetStartPosition() - startPosition);
			stream << beforePlaceholderString;
			stream << placeholder.GetReplacement(message, replacements);

			startPosition = placeholder.GetEndPosition();
			LogBreakIf(startPosition >= message.size());
		}

		if (startPosition < message.size())
		{
			stream << message.substr(startPosition);
		}

		return stream.str();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	Logger::Placeholder::Placeholder(std::size_t start, std::size_t length)
		: m_startPosition(start)
		, m_length(length)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::string Logger::Placeholder::GetReplacement(const std::string& message, const std::vector<std::string>& replacements) const
	{
		static const std::string INVALID_REPLACEMENT = "PLACEHOLDER";

		const auto endPosition = m_startPosition + m_length;
		LogReturnIf(endPosition > message.size() && "Invalid placeholder {0} - {1} in for message {2}", INVALID_REPLACEMENT, m_startPosition, endPosition, message);

		try
		{
			auto index = std::stoi(message.substr(m_startPosition, m_length));
			if (index < 0 || index >= replacements.size())
			{
				throw std::out_of_range("Invalid placeholder index for message: " + message);
			}

			return replacements[index];
		}
		catch (std::exception& e)
		{
			Logger::Get().LogError("Error while replacing placeholder caused by exception: {0}", e.what());
			return INVALID_REPLACEMENT;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::size_t Logger::Placeholder::GetStartPosition() const
	{
		assert(m_startPosition > 0);
		return m_startPosition - 1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::size_t Logger::Placeholder::GetEndPosition() const
	{
		return m_startPosition + m_length + 1;
	}
}