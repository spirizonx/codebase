#include <iostream>
#include <cassert>
#include "List.h"
#include<stdexcept>
using namespace std;

template <typename T>
class ArrayList : public List<T> 
{
    private:
        const static int defoultcap = 16;
        int cnt,cap,modCnt;
         T* array;
    public:
        ArrayList()
        {
            cnt=0; cap=defoultcap;
            modCnt=0;
            array=new T[cap];
        }
        ~ArrayList()
        {
            delete [] array;
        }
        int size() const
        {
            return cnt;
        }
        bool isEmpty() const
        {
            return cnt==0;
        }
        T get(int index) const
        {
            return *(array+index);
        }
        void add(const T& element)
        {
            modCnt++;
            if (cnt+1>cap)
            {
                cap*=2;
                T *newarr=new T[cap];
                for (int i=0;i<cnt;i++) newarr[i]=array[i];
                delete [] array;
                array=newarr;
            }
            *(array+cnt)=element;
            cnt++;
        }
        T remove(int index)
        {
            modCnt++;
            int tmp=array[index];
            for (int i=index;i<cnt-1;i++) array[i]=array[i+1];
            cnt--;
            if (cnt<cap/4&&cap>defoultcap)
            {
                cap/=4;
                T *newarr=new T[cap];
                for (int i=0;i<cnt;i++) newarr[i]=array[i];
                delete [] array;
                array=newarr;
            }
            return tmp;
        }

        template <typename E>
        class ArrayIterator : public Iterator<E> {
            private :
                int itCnt;
                int itModCnt;

                ArrayList<E>* outer;

                void modifiedCheck()
                {
                    if (itModCnt != outer->modCnt) 
                    {
                        throw logic_error("the host arrayList has been changed!");
                    }
                }
            public :
                ArrayIterator(ArrayList<E>* outer) {
                    itCnt = 0;
                    itModCnt = outer->modCnt;
                    this->outer = outer;
                }     
                
                bool hasNext()
                {
                    modifiedCheck();
                    return itCnt!=outer->size();
                }

                E next() 
                {
                    modifiedCheck();
                    itCnt++;
                    return outer->array[itCnt-1];
                }
                ~ArrayIterator() {
                    outer = 0;
                }
        };
        Iterator<T>* iterator() {
            return new ArrayIterator<int>(this);
        }
};
    
