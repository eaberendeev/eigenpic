// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#ifndef ARRAY_H_
#define ARRAY_H_

#include <util.h>


template <typename T> 
struct Array3D{

    Array3D(int n1, int n2, int n3){
       allocate(n1,n2,n3);
    }
    Array3D(const Vector3d& nn){
       allocate(nn[0],nn[1],nn[2]);
    }
    Array3D(Array3D &&other): _data{other._data}, 
                  _size1{other._size1},_size2{other._size2},_size3{other._size3} {
        other._data = nullptr;
        other._size1 = other._size2 = other._size3 =  0; 
    }
    Array3D(const Array3D &other): _size1{other._size1},_size2{other._size2},_size3{other._size3} { 
       	allocate( _size1,_size2,_size3 );
    	for(auto i = 0; i < capacity(); ++i){
            _data[i] = other._data[i] ;
        }
    }
    Array3D(){
        _data = nullptr;
        _size1 = _size2 =_size3 = 0.;
    }
    
    void allocate(int n1, int n2, int n3){
        _data = new T[ n1 * n2 * n3];
        _size1 = n1;
        _size2 = n2;
        _size3 = n3;
    }
    
    void clear(){
        for(auto i = 0; i < capacity(); ++i){
            _data[i] = 0.;
        }
    }

    void free(){
        if (_data != nullptr)
            delete[] _data;
        _size1 = _size2 =_size3 = 0.;
    }

    ~Array3D(){
       free();
    }
    
    Array3D<T>& operator=(T s) {
        for(auto i = 0; i < capacity(); ++i){
            _data[i] = s ;
        }
        return (*this);
    }
    
    Array3D<T>& operator=(const Array3D<T>& array3D) {
    	// Self-assignment check
    	if (this == &array3D)
        	return *this;
        // Checking the conformity of dimensions
       	assert( capacity() == array3D.capacity() );
        for(auto i = 0; i < capacity(); ++i){
            _data[i] = array3D._data[i] ;
        }
        return (*this);
    }   
    Array3D<T>& operator=(Array3D<T> &&array3D) {
    	// Self-assignment check
    	if (this == &array3D)
        	return *this;
        // Checking the conformity of dimensions
       	assert( capacity() == array3D.capacity() );
		delete[] _data;
       	_data = array3D._data;
        _size1 = array3D._size1;
        _size2 = array3D._size2;
        _size3 = array3D._size3;
        array3D._data = nullptr;
        array3D._size1 = array3D._size2 = array3D._size3 =  0; 
        return (*this);
    }   

    Array3D<T>& operator-=(const Array3D<T>& array3D) {
        assert( capacity() == array3D.capacity() );
        for(auto i = 0; i < capacity(); ++i){
            _data[i] -= array3D._data[i] ;
        }
        return (*this);
    }
    
    Array3D<T>& operator+=(const Array3D<T>& array3D) {
        assert( capacity() == array3D.capacity() );
        for(auto i = 0; i < capacity(); ++i){
            _data[i] += array3D._data[i] ;
        }
        return (*this);
    }          
    
    T& operator() (int i, int j, int k) {
        #if DEBUG > 0 
            if ( capacity() <= i * _size2 * _size3 + j * _size3 + k || i * _size2 * _size3 + j * _size3 + k < 0 ){
                std::string msg = "IndexError3";
                std::cout << msg << ": "<< Vector3i(i,j,k)<< "\n";
                throw msg;
            };
        #endif
    	return _data[i * _size2 * _size3 + j * _size3 + k ];
    }

    const T& operator() (int i, int j, int k) const{ 
        #if DEBUG > 0 
            if ( capacity() <= i * _size2 * _size3 + j * _size3 + k || i * _size2 * _size3 + j * _size3 + k < 0 ){
                std::string msg = "IndexError3";
                std::cout << msg << ": "<< Vector3i(i,j,k) << "\n";
                throw msg;
            };
        #endif
    	return _data[i * _size2 * _size3 + j * _size3 + k];
    }
    T& data(int i) {
        #if DEBUG > 0 
            if ( capacity() <= i || i < 0 ){
                std::string msg = "IndexError3";
                std::cout << msg << ": " << i << "\n";
                throw msg;
            };
        #endif
        return _data[i];
    }
    const T& data(int i) const {
        #if DEBUG > 0 
            if ( capacity() <= i || i < 0){
                std::string msg = "IndexError3";
                std::cout << msg << ": " << i << "\n";
                throw msg;
            };
        #endif
        return _data[i];
    }
  
    Vector3i size() const{
        return Vector3i(_size1,_size2,_size3);
    }
    int capacity() const{
        return _size1*_size2*_size3;
    }

protected:
    T* _data;
    int _size1, _size2, _size3;
};

template <typename T> 
struct Array2D{
    Array2D(int n1, int n2){
       allocate(n1,n2);
    }
    Array2D(Vector2i nn){
       allocate(nn(0),nn(1));
    }
    Array2D(Array2D &&other): _data{other._data}, _size1{other._size1}, _size2{other._size2}{
        other._data = nullptr;
        other._size1 = other._size2 = 0; 
    }
    Array2D(){
        _data = nullptr;
        _size1 = _size2 = 0; 
    }
    
    void allocate(int n1, int n2){
        _data = new T[ n1 * n2];
        _size1 = n1;
        _size2 = n2;
    }
    
    void clear(){
        for(auto i = 0; i < capacity(); ++i){
            _data[i] = 0.;
        }
    }
    void free(){
        if (_data != nullptr)
            delete[] _data;
        _size1 = _size2 = 0.;
    }
    
    ~Array2D(){
       free();
    }
    Array2D<T>& operator=(T s) {
        for(auto i = 0; i < capacity(); ++i){
            _data[i] = s ;
        }
        return (*this);
    }

    Array2D<T>& operator=(const Array2D<T>& array2D) {
        assert( capacity() == array2D.capacity() && "Array 2D sizes do not match!\n");
        for(auto i = 0; i < capacity(); ++i){
            _data[i] = array2D._data[i] ;
        }
        return (*this);
    }
    Array2D<T>& operator=(const Array2D<T>&& other) {
        //assert( capacity() == other.capacity() );
        
        if(&other == this) return (*this);

        free();
        _size1 = other._size1;
        _size2 = other._size2;
        _data = other._data;
        other._data = nullptr;
        other._size1 = other._size2 = 0;
        
        return (*this);
    }

    Array2D<T>& operator-=(const Array2D<T>& array2D) {
        assert( capacity() == array2D.capacity()  && "Array 2D sizes do not match!\n");
        for(auto i = 0; i < capacity(); ++i){
            _data[i] -= array2D._data[i] ;
        }
        return (*this);
    }
    
    Array2D<T>& operator+=(const Array2D<T>& array2D) {
        assert( capacity() == array2D.capacity()  && "Array 2D sizes do not match!\n");
        for(auto i = 0; i < capacity(); ++i){
            _data[i] += array2D._data[i] ;
        }
        return (*this);
    }

    T& operator() (int i, int j) {
        #if DEBUG > 0 
            if ( capacity() <= i * _size2  + j ||  i * _size2  + j < 0 ){
                std::string msg = "IndexError2";
                std::cout << msg << "\n";
                throw msg;
            };
        #endif
	return _data[i * _size2 + j];
    }

    const T& operator() (int i, int j) const{	
        #if DEBUG > 0 
            if ( capacity() <= i * _size2  + j ||  i * _size2  + j < 0 ){
                std::string msg = "IndexError2";
                std::cout << msg << "\n";
                throw msg;
            };
        #endif	
        return _data[i * _size2 + j];
    }
    
    T& data(int i) {
        #if DEBUG > 0 
            if ( capacity() <= i || i < 0 ){
                std::string msg = "IndexError2";
                std::cout << msg << "\n";
                throw msg;
            };
        #endif
        return _data[i];
    }
    const T& data(int i) const {
        #if DEBUG > 0 
            if ( capacity() <= i || i < 0){
                std::string msg = "IndexError2";
                std::cout << msg << "\n";
                throw msg;
            };
        #endif
        return _data[i];
    }
  
    Vector2i size() const{
        return Vector2i(_size1,_size2);
    }
    int capacity() const{
        return _size1*_size2;
    }
    T sum_d2(int i) const{
        T sum = 0;
        for( int t =  0; t < _size2; ++t){
          sum += _data[i * _size2 + t];
        }
        return sum;
    }

protected:
    T* _data;
    int _size1,_size2;
};

template <typename T> 
struct Array1D{
    
    Array1D(int size){
       allocate(size);
    }
    Array1D(Array1D &&other): _data{other._data}, _size{other._size} {
        other._data = nullptr;
        other._size = 0; 
    }
    Array1D(){
        _size = 0.;
    }
    
    void allocate(int sizeDim){
        _data = new T[ sizeDim];
        _size =  sizeDim;
    }
    
    void clear(){
        for(auto i = 0; i <_size; ++i){
            _data[i] = 0.;
        }
    }
    void free(){
        if (_data != nullptr)
            delete[] _data;
        _size = 0.;
    }
    
    Array1D<T>& operator=(T s) {
        for(auto i = 0; i < _size; ++i){
            _data[i] = s ;
        }
        return (*this);
    }

    Array1D<T>& operator=(const Array1D<T>& other) {
        assert( capacity() == other.capacity() && "Array 1D sizes do not match!\n");
        for(auto i = 0; i < capacity(); ++i){
            _data[i] = other._data[i] ;
        }
        return (*this);
    }

    Array1D<T>& operator-=(const Array1D<T>& other) {
        assert( capacity() == other.capacity()  && "Array 1D sizes do not match!\n");
        for(auto i = 0; i < capacity(); ++i){
            _data[i] -= other._data[i] ;
        }
        return (*this);
    }
    
    Array1D<T>& operator+=(const Array1D<T>& other) {
        assert( capacity() == other.capacity()  && "Array 1D sizes do not match!\n");
        for(auto i = 0; i < capacity(); ++i){
            _data[i] += other._data[i] ;
        }
        return (*this);
    }
    Array1D<T>& operator=(const Array1D<T>&& other) {
        
        if( &other == this) return (*this);

        free();
        _size = other._size;
        _data = other._data;
        other._data = nullptr;
        other._size = 0;
        return (*this);
    }
    ~Array1D(){
        free();
    }
    
    T& operator() (int i) {
        #if DEBUG > 0 
            if ( capacity() <= i ||  i < 0 ){
                std::string msg = "IndexError";
                std::cout << msg << ": index = " << i <<". capacity = "<< capacity() << "\n";
                throw msg;
            };
        #endif      
        return _data[i];
    }

    const T& operator() (int i) const{ 
        #if DEBUG > 0 
            if ( capacity() <= i ||  i < 0 ){
                std::string msg = "IndexError";
                std::cout << msg << "\n";
                throw msg;
            };
        #endif          
        return _data[i];
    }
    int size() const{
        return _size;
    }
    int capacity() const{
        return _size;
    }
    T& data(int i) {
        #if DEBUG > 0 
            if ( capacity() <= i ||  i < 0 ){
                std::string msg = "IndexError";
                std::cout << msg << "\n";
                throw msg;
            };
        #endif  
        return _data[i];
    }
    const T& data(int i) const {
        #if DEBUG > 0 
            if ( capacity() <= i ||  i < 0 ){
                std::string msg = "IndexError";
                std::cout << msg << "\n";
                throw msg;
            };
        #endif  
        return _data[i];
    }  

protected:
    T *_data;
    int _size;  
};

template <class T>
struct Array : std::vector<T> {
    using std::vector<T>::reserve;
    using std::vector<T>::size;
    using std::vector<T>::pop_back;
    using std::vector<T>::erase;
    T& operator() (long i) {
        return (*this)[i];
    }

    const T& operator() (long i) const{
        return (*this)[i];
    }
    void del(long k){
        (*this)[k] = (*this)[ size()-1];
        pop_back();
    }

};

#endif 
