#pragma once
#include "CMTypes.h"
#include <iostream>
#include <map>
#include <functional>
#include <vector>

using namespace std;

using CMEventSlot = function<void(const CMEvent&)>;

class CMEventDispatcher
{
public:
	void upload(const CMUInt& reference, CMEventSlot&& slot) { m_Container[reference].push_back(slot); }
	void post(const CMEvent& e) const
	{
		if (m_Container.find(e.value) == m_Container.end())
			return;
	
		auto&& elements = m_Container.at(e.value);
	
		for (auto&& el : elements)
			el(e);
	}
	void setVal(CMUInt& val) { CMEvent::value = val; }
private:
	map<CMUInt, vector<CMEventSlot>> m_Container;
};

void CMSendEventMsg(CMEvent msg, void (*callback)(const CMEvent&))
{
	using namespace std::placeholders;

	CMEventDispatcher d;

	d.setVal(msg.value);

	d.upload(msg.value, callback);

	d.post(CMEvent());
}