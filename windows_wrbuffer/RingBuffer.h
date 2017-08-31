#pragma once

#include "common.h"

typedef unsigned char BYTE;

// capacity
template <unsigned int N>
class RingBuffer {
public:
	typedef size_t size_type;
	typedef GenericLocker<CriticalSection> _BufferLocker;

	RingBuffer() : m_pushPos(0), m_popPos(0), m_count(0) {
		m_pRingBuffer = new BYTE[N];
	}
	~RingBuffer() { delete[] m_pRingBuffer; }

	bool is_full() const {
		_BufferLocker guard(m_mutex);
		return (m_count == N);
	}
	bool is_empty() const {
		_BufferLocker guard(m_mutex);
		return (m_count == 0);
	}
	size_type size() const {
		_BufferLocker guard(m_mutex);
		return m_count;
	}
	size_type capacity() const {
		return N;
	}
	size_type push(const BYTE* date, size_type length) {
		_BufferLocker guard(m_mutex);
		assert(date != NULL);

		// no date or will be full
		if (length == 0 || length > (N - m_count))
			return 0;
		size_type rearlen = N - m_pushPos; // length of from m_pushPos to end
		if (length <= rearlen) {
			::memmove(&m_pRingBuffer[m_pushPos], data, length);
			m_pushPos = (m_pushPos + length) % N;
		}
		else {
			::memmove(&m_pRingBuffer[m_pushPos], date, rearlen);
			::memmove(m_pRingBuffer, date + rearlen, length - rearlen);
			m_pushPos = length - rearlen;
		}

		m_count += length;

		return length;
	}

	size_type pop(BYTE* buf, size_type length) {
		_BufferLocker guard(m_mutex);
		assert(buf != NULL);

		if (length == 0 || length > m_count)
			return 0;

		size_type rearlen = N - m_popPos;
		if (length <= rearlen) {
			::memmove(buf, &m_pRingBuffer[m_popPos], length);
			m_popPos = (m_popPos + length) % N;
		}
		else {
			::memmove(buf, &m_pRingBuffer[m_popPos], rearlen);
			::memmove(buf + rearlen, m_pRingBuffer, length - rearlen);
			m_popPos = length - rearlen;
		}
		m_count -= length;
		return length;
	}

	void clear() {
		_BufferLocker guard(m_mutex);
		m_pushPos = 0, m_popPos = 0, m_count = 0;
	}
private:
	RingBuffer(const RingBuffer<N>&);
	RingBuffer& operator=(const RingBuffer<N>&);
private:
	BYTE *m_pRingBuffer; // buffer
	size_type m_pushPos; // push position, m_pushPos = (m_pushPos + 1)%N
	size_type m_popPos;  // pop position
	size_type m_count;   // size
	CriticalSection m_mutex;
};