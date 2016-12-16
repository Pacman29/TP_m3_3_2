#include <iostream>
#include <stack>
#include <queue>
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
    std::vector<size_t> layers(this->depth,0);
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
            layers[tmp->depth]++;
            st.pop();
            tmp = tmp->right;
        }
    }
    int max = 0;
    for(size_t i = 0; i<layers.size();++i)
        if(layers[i]>max)
            max = layers[i];
    return max;
}


template <typename T>
class decartTree
{
public:
    decartTree(): head(NULL) {}
    ~decartTree()
    {
        delete this->head;
    }

    void add(T value,size_t priority);
    int max_wide();
private:

    void split(node<T>* tree,T value,node<T> *&left,node<T> *&right);
    node<T>* merge(node<T>* left,node<T>* right);
    node<T>* head;
    void _add(node<T>*& _node, T value, size_t priority);
};

template <typename T>
void decartTree<T>::add(T value, size_t priority)
{
    this->_add(this->head,value,priority);
}

template <typename T>
int decartTree<T>::max_wide()
{
    if(!this->head)
        return 0;

    vector<int> count_on_layers;
    int sum_on_layer = 0;
    int depth = 0;
    queue <node<T>*> q;

    q.push(this->head);
    while (!q.empty())
    {
        node<T>* tmp_node= q.front();
        if(depth == tmp_node->depth)
            sum_on_layer++;
        else
        {
            count_on_layers.push_back(sum_on_layer);
            sum_on_layer = 1;
            depth = tmp_node->depth;
        }
        q.pop();
        if(tmp_node->left)
        {
            tmp_node->left->depth++;
            q.push(tmp_node->left);
        }
        if(tmp_node->right)
        {
            tmp_node->right->depth++;
            q.push(tmp_node->right);
        }
        tmp_node->depth = 0;
    }
    int max = count_on_layers[0];
    for(int i = 1; i<count_on_layers.size(); ++i)
        if(count_on_layers[i]>max)
            max = count_on_layers[i];
    return max;
}

template <typename T>
void decartTree<T>::split(node<T> *tree, T value, node<T> *&left, node<T> *&right)
{
    if(!tree)
        left = right = NULL;
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

template <typename T>
void decartTree<T>::_add(node<T> *&_node, T value, size_t priority)
{
    if(_node)
    {
        if(_node->priority < priority)
        {
            node<T> *left = NULL;
            node<T> *right = NULL;

            this->split(_node,value,left,right);

            node<T> *new_node = new node<T>(value,0,priority);
            new_node->left = left;
            new_node->right = right;
            _node = new_node;
            return;
        }
        if(_node->value > value)
            _add(_node->left,value,priority);
        else
            _add(_node->right,value,priority);
    }
    else
        _node = new node<T>(value,0,priority);
}


int main(void)
{
    int n = 0;
    cin>>n;
    binTree<int> b_tree(compare<int>);
    decartTree<int> d_tree;
    while(n)
    {
        int value,priority;
        cin>> value >> priority;
        b_tree.add(value);
        d_tree.add(value,priority);
        n--;
    }
    cout<< b_tree.max_wide() - d_tree.max_wide();
    return 0;
}
