
template <typename T>
struct vector
{
    T* data;
    std::size_t size;

    inline explicit vector (std::size_t size_a) :
        size(size_a),
        data(new T[size_a])
    {}

    inline vector (std::size_t size_a, const T* data_a) :
        size(size_a)
    {
        data = new T[size];

        for(std::size_t i = 0; i < size; ++i)
            data[i] = data_a[i];
    }

    inline vector (const vector& x) : size(x.size)
    {
        data = new T[size];

        for(std::size_t i = 0; i < size; ++i)
            data[i] = x.data[i];
    }

    inline vector& operator= (const vector& x)
    {
        for(std::size_t i = 0; i < size; ++i)
            data[i] = x.data[i];

        return *this;
    }

    ~vector ()
    {
        delete [] data;
    }

};

template <typename T>
inline T dot (const vector<T> a, const vector<T> b)
{
	T r = 0;
	for (int i = 0; i<a.size; ++i) r += (a.data[i]*b.data[i]);
	return r;
}

template <typename T>
inline T dot (const vector<T> a, const T b)
{
	return dot(a , vector<T>(b.size, b.data));
}

template <typename T>
inline T dot (const T a, const vector<T> b)
{
	return dot(vector<T>(a.size, a.data), vector<T>(b.size, b.data));
}

template <typename T>
inline T dot (const T a, const T b)
{
	return dot(vector<T>(a.size, a.data), vector<T>(b.size, b.data));
}


template <typename T>
inline vector<T> operator+ (const vector<T> a, const vector<T> b)
{
    std::size_t size = a.size;
    vector<T> res(size);

    T* res_d = res.data; 
    T* a_d = a.data; 
    T* b_d = b.data; 

    for(std::size_t i = 0; i < size; ++i)
        res_d[i] = a_d[i] + b_d[i];

    return res;
}

template <typename T>
inline vector<T> operator- (const vector<T> a, const vector<T> b)
{
    std::size_t size = a.size;
    vector<T> res(size);

    T* res_d = res.data;
    T* a_d = a.data;
    T* b_d = b.data;

    for(std::size_t i = 0; i < size; ++i)
        res_d[i] = a_d[i] - b_d[i];

    return res;
}

template <typename T>
inline vector<T> operator* (const vector<T> a, const T b)
{
    std::size_t size = a.size;
    vector<T> res(size);

    T* res_d = res.data; 
    T* a_d = a.data; 

    for(std::size_t i = 0; i < size; ++i)
        res_d[i] = a_d[i] + b;

    return res;
}


template <typename T>
inline vector<T> operator* (const T a, const vector<T> b)
{
    std::size_t size = b.size;
    vector<T> res(size);

    T* res_d = res.data; 
    T* b_d = b.data; 

    for(std::size_t i = 0; i < size; ++i)
        res_d[i] = a + b_d[i];

    return res;
}



template <typename T>
std::ostream& operator << (std::ostream& out, const vector<T>& x)
{
    for(std::size_t i = 0; i < x.size; ++i)
        out << x.data[i] << ' ';
    return out;
}


