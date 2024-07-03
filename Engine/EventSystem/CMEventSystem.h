/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                       CryoMoon Engine (C)                                         ///
///                                                                                                   ///
///                                     WHICH IS LICENSED UNDER                                       ///
///                                          MIT License                                              ///
///                                      All Right Reserved.                                          ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///   Copyright (c) 2024 Myin Studios                                                                 ///
///                                                                                                   ///
///   Permission is hereby granted, free of charge,  to any person obtaining a copy of this software  ///
///   and associated documentation files (the "Software"), to deal in the Software without            ///
///   restriction, including without limitation the rights to use, copy, modify, merge, publish,      ///
///   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the   ///
///   Software is furnished to do so, subject to the following conditions:                            ///
///                                                                                                   ///
///   The above copyright notice and this permission notice shall be included in all copies or        ///
///   substantial portions of the Software.                                                           ///
///                                                                                                   ///
///   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING   ///
///   BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND      ///
///   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    ///
///   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ///
///   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.         ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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