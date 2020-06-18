#pragma once
#include "ErrorHandling/ErrorCodes.h"

namespace chess
{
	class ValidationResult
	{
	public:
		bool IsValid() const;
		void AddError(ErrorCode error);

		ErrorCodes PopErrors();
	private:
		ErrorCodes m_errors;
	};
}