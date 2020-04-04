#pragma once

typedef int EventType;

template<typename EventType> struct EventTypeInfo;

#define DEFINE_EVENT_TYPE(EventClassName, EventTypeValue) \
template<> struct EventTypeInfo<EventClassName> \
{ \
	static constexpr EventType Type = EventTypeValue; \
}; \
extern template struct EventTypeInfo<EventClassName>;
