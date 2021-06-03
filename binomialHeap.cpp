// ConsoleApplication3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

class BinTree_node {
public:
    int data = 0;
    int key = 0;
    int degree = 0;
    BinTree_node* parent = NULL;
    BinTree_node* child = NULL;
    BinTree_node* sibling = NULL;
    BinTree_node() {}
};


void LinkedList_addEnd(BinTree_node* &h1, BinTree_node* h2) {
    if (h1){
        BinTree_node* last = h1;
        while (last->sibling) {
            last = last->sibling;
        }
        last->sibling = h2;
    } else {
        h1 = h2;
    }
}

void BinTreeLink(BinTree_node* child, BinTree_node* parent) {
    child->parent = parent;
    child->sibling = parent->child;
    parent->child = child;
    parent->degree += 1;
}

BinTree_node* BinHeapListMerge(BinTree_node* h1, BinTree_node* h2) {

    BinTree_node* h = nullptr;
    BinTree_node* sibling = nullptr;
    while (h1 && h2) {
        if (h1->degree <= h2->degree) {
            sibling = h1->sibling;
            h1->sibling = nullptr;
            LinkedList_addEnd(h, h1);
            h1 = sibling;
        } else {
            sibling = h2->sibling;
            h2->sibling = nullptr;
            LinkedList_addEnd(h, h2);
            h2 = sibling;
        }
    }

    while (h1) {
        sibling = h1->sibling;
        h1->sibling = nullptr;
        LinkedList_addEnd(h, h1);
        h1 = sibling;
    }

    while (h2) {
        sibling = h2->sibling;
        h2->sibling = nullptr;
        LinkedList_addEnd(h, h2);
        h2 = sibling;
    }

    return h;

};


BinTree_node* BinTreeHeapUnion(BinTree_node* h1, BinTree_node* h2) {
    BinTree_node* h = BinHeapListMerge(h1,h2);

    BinTree_node* prev_x = nullptr;
    BinTree_node* x = h;
    BinTree_node* next_x = x->sibling;

    while (next_x) {
        if ((x->degree != next_x->degree) || ((next_x->sibling) && (next_x->sibling->degree == x->degree))) {
            // 1 и 2 (протсо перемещаем по списку корней)
            prev_x = x;
            x = next_x;
        } else if (x->key <=next_x->key) {
            // 3 (next_x становится частью x)
            x->sibling = next_x->sibling;
            BinTreeLink(next_x, x);
        } else {
            // 4 (x становится частью next_x)
            if (!prev_x) {
                h = next_x;
            } else {
                prev_x->sibling = next_x;
            }
            BinTreeLink(x, next_x);
            x = next_x;
        }
        next_x = x->sibling;
    }

    return h;

}

BinTree_node* BinTreeInsert(BinTree_node* h, int key, int value) {
    BinTree_node* x = new BinTree_node();
    x->key = key;
    x->data = value;
    return BinTreeHeapUnion(h, x);
}

BinTree_node* BinHeapGetMin(BinTree_node* h) {
    BinTree_node* x = h;
    BinTree_node* minx = new BinTree_node();
    minx->key = 99999;// inf

    while (x) {
        if (x->key < minx->key) {
            minx = x;
        }
        x = x->sibling;
    }

    return minx;
}

BinTree_node* BinHeapDeleteMin(BinTree_node* &h) {
    BinTree_node* x = h;
    BinTree_node* minx = new BinTree_node();
    minx->key = 99999;// inf
    BinTree_node* prev = nullptr;
    BinTree_node* prevmin = nullptr;

    while (x) {
        if (x->key < minx->key) {
            minx = x;
            prevmin = prev;
        }
        prev = x;
        x = x->sibling;
    }

    if (prevmin) {
        prevmin->sibling = minx->sibling;
    } else {
        h = minx->sibling;
    }

    BinTree_node* child = minx->child;
    BinTree_node* sibling;
    prev = nullptr;

    while (child){
        sibling = child->sibling;
        child->sibling = prev;
        prev = child;
        child = sibling;
    }

    h = BinTreeHeapUnion(h, prev);

    return minx;
}

void BinHeapDecreaseKey(BinTree_node* x, int key) {

    if (x->key > key) {

        x->key = key;
        BinTree_node* y = x;
        BinTree_node* z = y->parent;
        int temp;

        while (z && (y->key < z->key) ) {
            temp = y->key;
            y->key = z->key;
            z->key = temp;
            y = z;
            z = y->parent;
        }

    } else {
        //error
        std::cout << "error: key only can be decreased\n";
    }

}

void BinHeapDelete(BinTree_node* &h, BinTree_node* x) {
    BinHeapDecreaseKey(x, -99999);// inf
    BinHeapDeleteMin(h);
};

int main()
{
    BinTree_node* n = new BinTree_node();
    /*
    n->data = 10;
    n->key = 8;

    BinTree_node* h = new BinTree_node();

    h->data = 5;
    h->key = 5;

    BinTree_node* r = BinTreeHeapUnion(n, h);

    BinTree_node* c = new BinTree_node();

    c->data = 36;
    c->key = 1;

    BinTree_node* r2  = BinTreeHeapUnion(r, c);

    BinHeapGetMin(r2);

    //BinTreeInsert(n, 1, 4);
    
    std::cout << r2->key;
    */

    n->key = 10;
    n->data = 10;
    //BinTreeInsert(n, 10, 10);
    n = BinTreeInsert(n, 12, 12);
    n = BinTreeInsert(n, 3, 3);
    n = BinTreeInsert(n, 56, 56);
    //BinTree_node* r = BinHeapGetMin(n);
    std::cout << n->key;
}

