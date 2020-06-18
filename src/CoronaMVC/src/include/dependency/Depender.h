#pragma once
#include "DependencyWrapper.h"

namespace mvc
{
	template<class... Dependencies>
	class Depender
	{
	private:
		template<class T>
		static T* GetInitializedDependency(const T* dependency)
		{
			return DependencyWrapper<T>::GetDependency();
		}

		template<std::size_t idx, class... Ts>
		struct DependencyInitializer
		{
			void operator() (std::tuple<Ts...>& tuple)
			{
				auto& dependency = std::get<idx>(tuple);
				dependency = GetInitializedDependency(dependency);
				DependencyInitializer<idx - 1, Ts...>{}(tuple);
			}
		};

		template<class... Ts>
		struct DependencyInitializer<0, Ts...>
		{
			void operator() (std::tuple<Ts...>& tuple) 
			{ 
				auto& dependency = std::get<0>(tuple);
				dependency = GetInitializedDependency(dependency);
			}
		};

	public:
		Depender() 
		{ 
			InitializeDependencies(); 
		}

		virtual ~Depender() = default;

	protected:
		template<class T>
		T& GetDependency() 
		{ 
			auto dependecy = std::get<T*>(m_dependencies);
			assert(dependecy);
			return *dependecy;
		}

		template<class T>
		const T& GetDependency() const 
		{ 
			auto dependecy = std::get<T*>(m_dependencies);
			assert(dependecy);
			return *dependecy;
		}

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