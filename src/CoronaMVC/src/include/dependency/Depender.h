#pragma once
#include "DependencyWrapper.h"

namespace mvc
{
	template<class... Dependencies>
	class Depender
	{
	private:
		template<class T>
		static void InitializeDependency(T*& dependency)
		{
			dependency = DependencyWrapper<T>::GetDependency();
		}

		template<std::size_t idx, class... Ts>
		struct DependencyInitializer
		{
			void operator() (std::tuple<Ts...>& tuple)
			{
				InitializeDependency(std::get<idx>(tuple));
				DependencyInitializer<idx - 1, Ts...>{}(tuple);
			}
		};

		template<class... Ts>
		struct DependencyInitializer<0, Ts...>
		{
			void operator() (std::tuple<Ts...>& tuple) { InitializeDependency(std::get<0>(tuple)); }
		};

	public:
		Depender() { InitializeDependencies(); }
		virtual ~Depender() = default;

	protected:
		template<class T>
		T& GetDependency() { return *std::get<T*>(m_dependencies); }

		template<class T>
		const T& GetDependency() const { return *std::get<T*>(m_dependencies); }

	private:
		void InitializeDependencies()
		{
			const auto size = std::tuple_size<std::tuple<Dependencies*...>>::value;
			DependencyInitializer<size - 1, Dependencies*...>{}(m_dependencies);
		}

	private:
		std::tuple<Dependencies *...> m_dependencies;
	};
}