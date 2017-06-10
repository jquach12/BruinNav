
/*

// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.


#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

#include <map>  // YOU MUST NOT USE THIS HEADER IN CODE YOU TURN IN

// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS TEMPLATE,
// since you are not allowed to use any STL associative containers, and
// this implementation uses std::map.



// This code is deliberately obfuscated.

// If you can not get your own MyMap class template working, you may use
// this one during development in order to let you proceed with implementing
// the other classes for this project; you can then go back to working on
// fixing your own MyMap class template.

template <typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap(const MyMap &other) = delete;
    MyMap &operator=(const MyMap &other) = delete;
    typedef KeyType O;
    using O1=int;
    void clear()
    {l01.clear();}
    using l0=ValueType;MyMap(){}using l10=O const;O1 size()
    const{return l01.size();}using ll0=l0 const;using Const=
    MyMap<O,l0>;void associate(l10&Using,ll0&first){l01[
                                                        Using]=first;}using l1=std::map<O,l0>;using l00=Const
    const;ll0*find(l10&l11)const{auto first=l01.find(l11);
        return(first!=l01.end()?&first->second:0);}l0*find(l10&
                                                           l01){return(l0*)(*(l00*)(this)).find(l01);}private:l1
    l01;
};


#endif // MYMAP_INCLUDED
*/

#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED



// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#include <string>
#include <vector>
#include <iostream>
#include "provided.h"

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    
    MyMap();
    ~MyMap();
    void clear();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);
    
    const ValueType* find(const KeyType& key) const;
    
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
   
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    
    
    //alright, this is basically a better linked list
    struct Node{
        
        //every node has two children, left and right
        Node* left = nullptr;
        Node* right = nullptr;
        
        KeyType m_key;
        ValueType m_value;
        
    };
    
    
    void freeTree(Node* cur){
        if (cur == nullptr)
            return;
        
        
        
        freeTree(cur->left);
        freeTree(cur->right);
        
        delete cur;
        --m_size; //dont forget the tree size gets smaller for every free'd node
        //cerr<<"Chopped off a branch! Tree size is now: "<<m_size<<endl;
    }
    
    
   const ValueType* finder(Node* curr, const KeyType& someKey) const{
        
        if(curr ==nullptr)
            return nullptr;
        
        if(curr->m_key == someKey){
            ValueType* something = &curr->m_value;
            return something;
        }
        
        if(curr->m_key < someKey){
            return finder(curr->right,someKey);
        }
        
        if(curr->m_key > someKey)
            return finder(curr->left,someKey);
        
       return nullptr;
    }
    
    
    Node* m_root;
    int m_size = 0; // we probably wanna record this
    
    
};


template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap(){
    m_root = nullptr;
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const{
    return m_size;
}



template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear(){
    freeTree(m_root);
}


template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap(){
    clear();
}




template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const{
     return finder(m_root, key);
    
}


template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>:: associate(const KeyType& key, const ValueType& value){
    
    
    //Check if inserting into an empty tree
    if(m_root == nullptr){
        m_root = new Node;
        m_root->m_key = key;
        m_root->m_value = value;
        m_size++;
        
        return;
    }
    

    
    Node *cur = m_root;
    
    
    for(;;){
        
        
        if(cur->m_key == key){
            cur->m_value = value;
            return;
        }
        else if (key < cur->m_key){
            if(cur->left == nullptr){
                cur->left = new Node;
                cur->left->m_key = key;
                cur->left->m_value = value;
                ++m_size;
                return;
            }
            else
                cur = cur->left;
        }
        
        else if (key > cur->m_key){
            if(cur->right == nullptr){
                cur->right = new Node;
                cur->right->m_key = key;
                cur->right->m_value = value;
                ++m_size;
                return;
            }
            else
                cur = cur->right;
        }
        
    }



}
#endif // MYMAP_INCLUDED
