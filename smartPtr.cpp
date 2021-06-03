// ConsoleApplication2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

class risc {
private:
    int *array;
    int counter;
public:
    risc(int size) {
        array = new int[size];
        counter++;
    }
    int getCounter() {
        return counter;
    }
    risc* newInstance() {
        counter++;
        return this;
    }

    int& operator [](int i) {
        return array[i];
    }
    int get(int i) {
        return array[i];
    }
    void onRemove() {
        counter--;
        if (counter == 0)
        {
            delete array;
        }
    }

};

void Free(risc* &r){
    r->onRemove();
    r = new risc(1);
    delete r;
}

int main()
{

    risc* r = new risc(2);

    std::cout << (*r).getCounter() << "\n";

    (*r)[0] = 1;
    (*r)[1] = 2;

    risc* r2 = (*r).newInstance();

    Free(r2);
    //Free(r);

    std::cout << (*r)[1] << " "<< (*r).getCounter();
}
