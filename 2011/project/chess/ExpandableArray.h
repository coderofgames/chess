#ifndef EXPANDABLE_ARRAY_H
#define EXPANDABLE_ARRAY_H
template <class T> 
class ExpandingArrayList
{
public:

	ExpandingArrayList<T>()
	{
		mData = 0;
	}

	ExpandingArrayList<T> (int size )
	{
		m_size = size;
		m_maxElem = 0;
		mData = new T[m_size];
	}

	~ExpandingArrayList<T>()
	{
		if( mData )
		{
			delete [] mData;
		}
	}

	void Allocate(int size)
	{
		m_size = size;
		m_maxElem = 0;
		mData = new T[m_size];
	}

	T& operator[](int i)
	{
		if( i < m_maxElem )
		{
			return mData[i];
		}
		else
		{
			return nullNode;
		}
	}

	
	void push_back_ref( T& elem )
	{
		if( m_maxElem >= m_size )
		{
			T* tempData = new T[m_size*2];
			
			for( int i = 0; i < m_size; i++ )
			{
				tempData[i] = mData[i];
			}

			delete [] mData;

			mData = tempData;

			mData[m_size] = elem;
			m_maxElem = m_size;

			m_size*=2;
		}
		else
		{
			mData[m_maxElem] = elem;
			m_maxElem++;
		}
	}

	void push_back( T elem )
	{
		if( m_maxElem >= m_size )
		{
			T* tempData = new T[m_size*2];
			
			for( int i = 0; i < m_size; i++ )
			{
				tempData[i] = mData[i];
			}

			delete [] mData;

			mData = tempData;

			mData[m_size] = elem;
			m_maxElem = m_size;

			m_size*=2;
		}
		else
		{
			mData[m_maxElem] = elem;
			m_maxElem++;
		}
	}

	bool GrowToSize(int sz)
	{
		if( sz < m_size )
		{
			return false;
		}

		T* tempData = new T[sz];
			
		for( int i = 0; i < m_size; i++ )
		{
			tempData[i] = mData[i];
		}

		delete [] mData;

		mData = tempData;

		m_maxElem = m_size;

		m_size= sz;

		return true;
	}

	void clear()
	{
		m_maxElem = 0;
	}
	

	int size()
	{
		return m_maxElem;
	}

	int m_maxElem;
	int m_size;
	T* mData;
	T nullNode;
};

/*
void radix_sort()
{
// Biggest number?
int max=-1;
for(int i=0;i<N;++i)
        if(a[i]>max)
                max = a[i];

// How many digits in it
int maxdigits=1;
while(max /= 10) maxdigits++;

// Create some buckets.
deque<int> b[10];
for(int i=0;i<10;++i)
        b[i] = deque<int>(N);

int div=1;
// Radix Sort by digits
for(int d=1;d<=maxdigits;++d)
{
        if(d>1)
                div*=10;

        // Queue
        for(int i=0;i<N;++i)
                b[ (a[i]/div) % 10 ].push_front(a[i]);

        // Dequeue
        int k=0;        
        for(int q=0;q<10;++q)
                while(b[q].size() > 0)
                {
                        a[k++] = b[q].back();
                        b[q].pop_back();
                }
}
}
*/
#endif