#include <iostream>
#include <stack>
#include <vector>
using namespace std;

template <typename T>
bool compare(T value1, T value2)
{
    return value1 <= value2;
}

template <typename T>
class node
{
public:
    node(T& data): value(data),left(NULL),right(NULL) {}
    node(T& data, size_t depth = 0, int priority = 0): value(data),priority(priority),left(NULL),right(NULL),depth(depth) {}
    ~node()
    {
        delete left;
        delete right;
    }

    node* left;
    node* right;
    T value;
    int priority;
    size_t depth;
};


template <typename T, typename node_t = node<T>>
class binTree
{
public:
    binTree(bool (*compare_func)(T,T)): head(NULL),comp(compare_func) {}
    ~binTree()
    {
        delete this->head;
    }

    int add(T value);
    int print_in_order();
    int max_wide();
private:
    size_t depth;
    bool (*comp)(T,T);
    node_t* head;
};


template <typename T, typename node_t>
int binTree<T,node_t>::add(T value)
{
    node_t** tmp = &this->head;
    size_t depth = 0;
    while (*tmp)
    {
        tmp = (comp((*tmp)->value,value)) ? (&((*tmp)->right)) : (&((*tmp)->left));
        depth++;
    }
    if(depth > this->depth)
        this->depth = depth;

    *tmp = new node_t(value,depth);
    if(!*tmp)
        return 1;
    return 0;
}


template <typename T, typename node_t>
int  binTree<T,node_t>::print_in_order()
{
    std::stack<node_t*> st;
    node_t* tmp = this->head;
    while(!st.empty() || tmp)
    {
        if(tmp)
        {
            st.push(tmp);
            tmp = tmp->left;
        }
        else
        {
            tmp = st.top();
            cout << tmp->value;
            st.pop();
            tmp = tmp->right;
            if(!st.empty() || tmp)
                cout << " ";
        }
    }
}

template <typename T, typename node_t>
int binTree<T,node_t>::max_wide()
{
    std::vector<size_t> layouts(this->depth,0);
    std::stack<node_t*> st;
    node_t* tmp = this->head;
    while(!st.empty() || tmp)
    {
        if(tmp)
        {
            st.push(tmp);
            tmp = tmp->left;
        }
        else
        {
            tmp = st.top();
            layouts[tmp->depth]++;
            st.pop();
            tmp = tmp->right;
        }
    }
    int max = 0;
    for(size_t i = 0; i<layouts.size();++i)
        if(layouts[i]>max)
            max = layouts[i];
    return max;
}


template <typename T>
class decartTree
{
public:
    decartTree(): depth(0),head(NULL) {}
    int add(T value,size_t priority);
private:

    void split(node<T>* tree,T value,node<T> *&left,node<T> *&right);
    node<T>* merge(node<T>* left,node<T>* right);

    size_t depth;
    node<T>* head;
};

template <typename T>
void decartTree<T>::split(node<T> *tree, T value, node<T> *&left, node<T> *&right)
{
    if(!tree)
    {
        left = right = NULL;
    }
    else
    {
        if(value >= tree->value)
        {
            split(tree->right,value,tree->right,right);
            left = tree;
        }
        else
        {
            split(tree->left,value,left,tree->left);
            right = tree;
        }
    }
}

template <typename T>
node<T> *decartTree<T>::merge(node<T> *left, node<T> *right)
{
    if(!left || !right)
        return (!left) ? (right) : (left);
    if (left->priority > right->priority)
    {
        left->right = merge(left->right, right);
        return left;
    }
    right->left = merge(left, right->left);
    return right;
}


int main(void)
{
    int n = 0;
    cin>>n;
    binTree<int> tree(compare<int>);
    while(n)
    {
        int value;
        cin>> value;
        tree.add(value);
        n--;
    }
    cout<<tree.max_wide();
    return 0;
}