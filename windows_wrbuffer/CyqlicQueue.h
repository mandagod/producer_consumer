#pragma once

template <typename T, unsigned int N>
class CyclicQueue {
public:
	typedef T  value_type;
	typedef size_t size_type;
	typedef T& refrence;
	typedef const T& const_refrence;

	CyclicQueue() : m_popPos(0), m_count(0) {
		assert(N > 0);
		m_beginPtr = (T*)(::operator new(sizeof(T) * N));
	}

	~CyclicQueue() {
		_Clear();
		::operator delete((void*)m_beginPtr);
	}

	CyclicQueue(const CyclicQueue<T, N>& copy) :m_popPos(0), m_count(0) {
		assert(N > 0);
		m_beginPtr = (T*)(::operator new(sizeof(T) * N));
		size_t copypos = copy.m_popPos;
		for (size_type idx = 0; idx < copy.m_count, ++idx) {
			_Copy(idx, copy.m_beginPtr[copypos]);
			++copyPos; copypos %= N; ++m_count;
		}
	}
	CyclicQueue& operator=(const CCyclicQueue<T, N>& other) {
		CyclicQueue<T, N> temp(other);
		swap(temp);
		return (*this);
	}
	bool is_empty() const {
		return (m_count == 0);
	}
	bool is_full() const { return (m_count ==N); }
	value_type front() {
		assert(m_count != 0);
		return(m_beginPtr[m_popPos]);
	}
	value_type front() const {
		assert(m_count != 0);
		return(m_beginPtr[m_popPos]);
	}
	value_type back() {
		assert(m_count != 0);
		size_type pushPos = (m_popPos + m_count) % N;
		if (pushPos == 0)
			return (*(m_beginPtr + N - 1));

		return (m_beginPtr[pushPos - 1]);
	}
	value_type back() const {
		assert(m_count != 0);
		size_type pushPos = (m_popPos + m_count) % N;
		if (pushPos == 0)
			return (*(m_beginPtr + N - 1));

		return (m_beginPtr[pushPos - 1]);
	}
	bool push(const_refrence data = T()) {
		if (m_count < N) {
			size_type pushPos = (m_popPos + m_count) % N;
			_Copy(pushPos, data);
			++m_count;
			return true;
		}

		return false;
	}
	bool pop(refrence data) {
		if (m_count > 0) {
			data = m_beginPtr[m_popPos];
			_Destroy(m_popPos);
			--m_count; ++m_popPos; m_popPos %= N;
			return true;
		}
		return false;
	}
	size_type size() const { return m_count; }
	size_type capacity() const { return N; }
	void clear() { _Clear(); }
private:
	void _Copy(size_type idx, const_refrence) {
		assert(idx < N);
		T *pTemp = (m_beginPtr + idx);
		new ((void*)pTemp) T(data);
	}
	void _Destroy(size_type idx) {
		assert(idx < N);
		T *pTemp = (m_beginPtr + idx);
		pTemp->~T();
	}
	void _Clear() {
		for (; m_count > 0; --m_count) {
			_Destroy(m_popPos);
			++m_popPos; m_popPos %= N;
		}
		m_popPos = 0;
	}
	void swap(CyclicQueue<T, N>& other) {
		std::swap(m_beginPtr, other.m_beginPtr);
		std::swap(m_popPos, other.m_popPos);
		std::swap(m_count, other.m_count);
	}
private:
	value_type *m_beginPtr;
	size_type  m_popPos;
	size_type  m_count;
};