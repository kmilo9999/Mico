#pragma once
#include <vector>
#include <functional>

template<typename R, typename... Args>
class Signal
{
private:
	typedef std::function<T> Slot;

public:
	Signal() {}
	~Signal() {}
	void connect(Slot& slot) {
		slots.push_back(slot);
	}

	
	R emit(Args... args) {
		Slot slot;
		for (auto it = slots.begin(); it != slots.end();it++)
		{
			slot = (*it);
			return slot(args);
		}		
	}
	


private:
	std::vector<Slot> slots;
};

