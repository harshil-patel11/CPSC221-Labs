/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

/*
pop left
    +   +   +   -   -   -
n2  1   2   3   3   3   3
n1  0   0   0   1   2   3

pop right
    +   +   +   -   -   -
n2  1   2   3   2   1   0
n1  0   0   0   0   0   0
*/


template <class T>
Deque<T>::Deque(){

    data = {};
    n1 = 0;
    n2 = 0;
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem) {
    data.push_back(newItem);
    n2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL() {
    T left = peekL();
    n1++;
    
    if (isEmpty()) {
        data.resize(0);
        n1 = 0;
        n2 = 0;
    } 
    else if (data.size() - n1 <= n1) {
        vector<T> temp = {};
        for (unsigned long i = n1; i < data.size(); i++) {
            temp.push_back(data[i]);            
        }
        data = temp;
        n2 = temp.size();
        n1 = 0;        
    }

    return left;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR() {
    T right = peekR();
    data.pop_back();
    n2--;

    if (isEmpty()) {
        data.resize(0);
        n1 = 0;
        n2 = 0;
    } 
    else if (data.size() - n1 <= n1) {
        vector<T> temp = {};
        for (unsigned long i = n1; i < data.size(); i++) {
            temp.push_back(data[i]);            
        }
        data = temp;
        n2 = temp.size();
        n1 = 0;        
    }

    return right;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL() {
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR() {
    return data[n2-1];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const {
    if (n1 == n2) {
        return true;
    }
    return false;
}
