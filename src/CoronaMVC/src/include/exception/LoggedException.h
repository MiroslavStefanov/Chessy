#pragma once

namespace mvc
{
	class LoggedException : public std::runtime_error
	{
	public:
		LoggedException(std::string message) noexcept;
		LoggedException(std::string message, const std::exception& cause) noexcept;

		virtual const char* what() const noexcept override;
		const std::exception* GetCause() const;

	protected:
		const std::exception* m_cause;
		std::string m_message;
	};
}