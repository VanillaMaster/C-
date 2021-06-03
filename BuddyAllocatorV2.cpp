// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;

class memoryInsance {
public:
    int startIndex;
    int endIndex;
    int getSize() {
        return (endIndex - startIndex + 1);
    }
    int getNextSplitSize() {
        return (getSize() / 2);
    }
    memoryInsance(int startIndex, int endIndex) {
        this->startIndex = startIndex;
        this->endIndex = endIndex;
    }
    memoryInsance() {
    }
};

template <class T>
class risc {
private:
    T* result;
    int startIndex;
    int endIndex;
public:
    risc(T mx[],int startIndex,int endIndex) {
        this->result = { mx };
        this->startIndex = startIndex;
        this->endIndex = endIndex;
    }
    T &operator [](int i) { // const
        if ((i >= 0) && (i <= (endIndex - startIndex))) {
            return result[startIndex+i];
        }
        throw out_of_range("");
    }

    int length() {
        return (endIndex - startIndex + 1);
    }

    bool eaqualToMemoryInsance(memoryInsance item){
        if ((item.endIndex == endIndex) && (item.startIndex == startIndex)) {
            return true;
        } else {
            return false;
        }
    }
};

template <class T>
class list {
private:
    T* innerArray = { new T[1] };
    T* innerSubArray = { new T[1] };
    int length = 1;
    int pointer = 0;

    void extend() {
        innerSubArray = { new T[length * 2] };
        for (int i = 0; i < length; i++) {
            innerSubArray[i] = innerArray[i];
        }
        length *= 2;
        innerArray = { innerSubArray };
        innerSubArray = { new T[0] };
    }

public:
    void deleteAt(int index) {
        innerSubArray = { new T[(length - 1)] };
        for (int i = 0; i < (length - 1); i++) {
            if (i >= index)
            {
                innerSubArray[i] = innerArray[(i+1)];
            }
            else {
                innerSubArray[i] = innerArray[i];
            }
        }
        pointer--;
        innerArray = { innerSubArray };
        innerSubArray = { new T[0] };
    }
    void push(T item) {
        innerArray[pointer] = item;
        pointer++;
        if (pointer == length) {
            extend();
        }
    }
    T operator [](int i) const {
        if (i < length) {
            return innerArray[i];
        }
        throw out_of_range("");
    }
    int getLength() {
        return length;
    }
    int getSize() {
        return pointer;
    }
    bool isEmpty() {
        if (pointer == 0){
            return true;
        } else {
            return false;
        }
    }
};

class aloc {
private:
    char* byteArray;
    int size;

    list<memoryInsance> usedMemory;
    list<memoryInsance> freeMemory;

    void concatMemory() {
        //cout << "size:" << freeMemory.getSize() << "\n";

        if (freeMemory.getSize() > 1)
        {
            for (int i = (freeMemory.getSize() - 1); i > 0; i--) {
                if (freeMemory[i].getSize() == freeMemory[i - 1].getSize()) {
                    //cout << i << " and " << (i-1) <<"\n";

                    memoryInsance m = memoryInsance(freeMemory[i - 1].startIndex, freeMemory[i].endIndex);
                    freeMemory.deleteAt(i);
                    freeMemory.deleteAt((i-1));
                    freeMemory.push(m);
                    return concatMemory();
                }
            }
        }

    }

public:
    aloc(int size) {
        byteArray = { new char[size] };
        this->size = size;
        for (int i = 0; i < size; i++) {
            byteArray[i] = '-';
        }
        freeMemory.push(memoryInsance(0, (size - 1)));
    }


    risc<char> getMemory(int size) {

        int memoryIndex = -1;
        memoryInsance memory = memoryInsance(0, (this->size - 1));

        if (memory.getSize() < size)
        {
            throw length_error("not enought size");
        }

        for (int i = 0; i < freeMemory.getSize(); i++) {
            if ((freeMemory[i].getSize() <= memory.getSize()) && (freeMemory[i].getSize() >= size)) {
                memory = freeMemory[i];
                memoryIndex = i;
            }
        }

        if (memoryIndex == -1)
        {
            throw exception("no free space");
        }

        if (memory.getNextSplitSize() > size) {
            //cout << "can be splited" << "\n";
            memoryInsance m1 = memoryInsance(memory.startIndex, (memory.startIndex + memory.getNextSplitSize() - 1));
            memoryInsance m2 = memoryInsance((memory.startIndex + memory.getNextSplitSize()), memory.endIndex);
            freeMemory.deleteAt(memoryIndex);
            freeMemory.push(m1);
            freeMemory.push(m2);
            //cout << memoryIndex;
            return getMemory(size);
        }
        else {
            //cout << "can not be splited x:" << freeMemory[memoryIndex].startIndex << " y:" << freeMemory[memoryIndex].endIndex << "\n";
            risc<char> r(byteArray, freeMemory[memoryIndex].startIndex, freeMemory[memoryIndex].endIndex);
            usedMemory.push(freeMemory[memoryIndex]);
            freeMemory.deleteAt(memoryIndex);
            return r;
        }

    }

    bool deleteMemeoryInstance(risc<char> instance) {
    
        if (!usedMemory.isEmpty())
        {
            for (int i = 0; i < usedMemory.getSize(); i++) {
                //cout << instance.eaqualToMemoryInsance(usedMemory[i]);
                if (instance.eaqualToMemoryInsance(usedMemory[i]))
                {

                    for (int j = usedMemory[i].startIndex; j <= usedMemory[i].endIndex; j++) {
                        byteArray[j] = '-';
                    }

                    freeMemory.push(usedMemory[i]);
                    usedMemory.deleteAt(i);

                    concatMemory();

                    return true;
                }
            }
            return false;
        }
        else {
            cout << "empty\n";
            return false;
        }

    }


    void vizualize() {
        cout << "freeMemory:\n";
        if (!freeMemory.isEmpty())
        {
            for (int i = 0; i < freeMemory.getSize(); i++) {
                cout << "s:" << freeMemory[i].startIndex << " e:" << freeMemory[i].endIndex << " size:" << freeMemory[i].getSize() << "\n";
            }
        }
        else
        {
            cout << "empty\n";
        }

        cout << "usedMemory:\n";
        if (!usedMemory.isEmpty())
        {
            for (int i = 0; i < usedMemory.getSize(); i++) {
                cout << "s:" << usedMemory[i].startIndex << " e:" << usedMemory[i].endIndex << " size:" << usedMemory[i].getSize() << "\n";
            }
        }
        else {
            cout << "empty\n";
        }

        cout << "by bits:\n";
        for (int i = 0; i < size; i++) {
            cout << byteArray[i];
        }
        cout << "\n";
    }
};

int main()
{

    aloc al = aloc(64);

    al.vizualize();
    cout << "\n";
    
    risc<char> r = al.getMemory(7);

    for (int i = 0; i < r.length(); i++) {
        r[i] = 'a';
    }

    al.vizualize();
    cout << "\n";

    risc<char> r2 = al.getMemory(32);

    for (int i = 0; i < r2.length(); i++) {
        r2[i]= 'b';
    }

    al.vizualize();
    cout << "\n";

    al.deleteMemeoryInstance(r);

    al.vizualize();

}
