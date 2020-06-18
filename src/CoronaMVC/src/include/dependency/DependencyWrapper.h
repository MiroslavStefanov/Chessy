#pragma once

namespace mvc
{
	template<class T>
	class DependencyWrapper
	{
	private:
		static DependencyWrapper<T>* s_instance;

	public:
		static T* GetDependency();

	public:
		DependencyWrapper<T>();

		T* operator->();
		T& operator*();

	private:
		void Initialize();

	private:
		std::unique_ptr<T> m_dependency;
		bool m_initializationRequested;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class T>
	DependencyWrapper<T>* DependencyWrapper<T>::s_instance = nullptr;

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class T>
	inline T* DependencyWrapper<T>::GetDependency()
	{
		assert(s_instance);

		auto& dependency = **s_instance;
		return &dependency;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class T>
	inline DependencyWrapper<T>::DependencyWrapper() : m_initializationRequested(false)
	{
		LogReturnIf(s_instance && "Duplicated dependency", VOID_RETURN);
		s_instance = this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class T>
	inline T* DependencyWrapper<T>::operator->()
	{
		if (!m_dependency)
		{
			Initialize();
		}

		return m_dependency.get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class T>
	inline T& DependencyWrapper<T>::operator*()
	{
		if (!m_dependency)
		{
			Initialize();
		}

		return *m_dependency;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class T>
	inline void DependencyWrapper<T>::Initialize()
	{
		assert(!m_initializationRequested); //Assert that there are no cyclic dependencies

		m_initializationRequested = true;
		m_dependency = std::make_unique<T>();

		m_initializationRequested = false;
	}
}