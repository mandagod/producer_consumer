#include "common.h"

#include<deque>

template <typename T>
class SyncQueue {
public:
	typedef typename std::deque<T>::size_type size_type;
	typedef typename std::deque<T>::value_type value_type;
	typedef GenericLocker<CriticalSection> _QueueLocker;

	size_type size() const {
		_QueueLocker guard(m_mutex);
		return m_deque.size();
	}
	bool empty() const {
		_QueueLocker guard(m_mutex);
		return m_deque.empty();
	}
	void clear() {
		_QueueLocker guard(m_mutex);
		m_deque.clear();
	}
	void push(const value_type& x) {
		_QueueLocker guard(m_mutex);
		m_deque.push_back(x);
	}
	bool pop(value_type& value) {
		_QueueLocker guard(m_mutex);
		if (m_deque.empty())
			return false;
		value = m_deque.front();
		m_deque.pop_front();
		return true;
	}
private:
	// use dqueue
	std::deque<T> m_deque;
	CriticalSection m_mutex;
};