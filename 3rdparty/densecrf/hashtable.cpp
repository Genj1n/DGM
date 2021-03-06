#include "hashtable.h"

// Constructor
CHashTable::CHashTable(int key_size, size_t nElements) : m_key_size(key_size), m_capacity(2 * nElements), m_filled(0)
{
	m_pTable = new int[m_capacity];
	m_pKeys = new short[(m_capacity / 2 + 10) * m_key_size];
	memset(m_pTable, -1, m_capacity * sizeof(int));
}

// Destructor
CHashTable::~CHashTable(void)
{
	delete[] m_pKeys;
	delete[] m_pTable;
}

void CHashTable::reset(void)
{
	m_filled = 0;
	memset(m_pTable, -1, m_capacity * sizeof(int));
}

// if add: returns list size - 1
int CHashTable::find(const short *key, bool create)
{
	if (2 * m_filled >= m_capacity) grow();
	
	// Get the hash value
	size_t h = hash(key) % m_capacity;
	
	// Find the element with he right key, using linear probing
	while (true) {
		int e = m_pTable[h];
		if (e == -1) {
			if (create) {
				// Insert a new key and return the new id
				for (size_t i = 0; i < m_key_size; i++)
					m_pKeys[m_filled * m_key_size + i] = key[i];
				m_pTable[h] = static_cast<int>(m_filled);
				m_filled++;
				return m_pTable[h];
			}
			else
				return -1;
		}
		
		// Check if the current key is The One
		bool good = true;
		
		for (size_t i = 0; i < m_key_size && good; i++)
			if (m_pKeys[e * m_key_size + i] != key[i])
				good = false;
		
		if (good) return e;
		
		// Continue searching
		h++;
		if (h == m_capacity) h = 0;
	}
}

void CHashTable::grow(void)
{
	// Swap out the old memory
	short	*old_keys  = m_pKeys;
	int		*old_table = m_pTable;
	size_t	old_capacity = m_capacity;
	m_capacity *= 2;
	// Allocate the new memory
	m_pKeys = new short[(old_capacity + 10) * m_key_size];
	m_pTable = new int[m_capacity];
	memset(m_pTable, -1, m_capacity * sizeof(int));
	memcpy(m_pKeys, old_keys, m_filled * m_key_size * sizeof(short));

	// Reinsert each element
	for (int i = 0; i < old_capacity; i++)
		if (old_table[i] >= 0) {
			int e = old_table[i];
			size_t h = hash(old_keys + (getKey(e) - m_pKeys)) % m_capacity;
			for (; m_pTable[h] >= 0; h = h < m_capacity - 1 ? h + 1 : 0);
			m_pTable[h] = e;
		}

	delete[] old_keys;
	delete[] old_table;
}

size_t CHashTable::hash(const short *key)
{
	size_t res = 0;
	for (size_t i = 0; i < m_key_size; i++) {
		res += key[i];
		res *= 1664525;
	}
	return res;
}
