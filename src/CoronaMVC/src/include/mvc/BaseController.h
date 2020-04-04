#pragma once

namespace mvc
{
	class ApplicationStartedEvent;

	class BaseController
	{
	public:
		virtual ~BaseController();

		void			RegisterToDispatcher(class EventDispatcher& dispatcher);
		ModelAndView	ConsumeEvent(const Event& event) const;
		
	protected:
		virtual void			RegisterConsumers();
		virtual ModelAndView	OnApplicationStartedEvent(ApplicationStartedEvent const& event);

		template<class EventClass> 
		void RegisterConsumer(std::function<ModelAndView(const EventClass&)> consumer);

	private:
		void RegisterConsumer(EventType eventType, std::function<ModelAndView(const Event&)>);

	private:
		std::unordered_map<EventType, std::function<ModelAndView(const Event&)>> m_eventConsumers;
	};

	template<class EventClass>
	inline void BaseController::RegisterConsumer(std::function<ModelAndView(const EventClass&)> consumer)
	{
		RegisterConsumer(EventTypeInfo<EventClass>::Type, [consumer](const Event& event) {return consumer(static_cast<const EventClass&>(event)); });
	}
}