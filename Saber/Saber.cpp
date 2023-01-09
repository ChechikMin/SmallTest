#pragma warning(disable : 4996)
///test only
#include <bitset>
#include <cassert>
/////////////////
#include <type_traits>
#include <iostream>
#include <io.h>
#include <limits>
#define _CRT_NONSTDC_NO_WARNINGS
///////////////////////////////////////////////////////////////1.
template <typename T>
void process(const T&& digit) 
{
    T flag = std::numeric_limits<T>::max() / 2 + 1;

    while (flag)
    {
        if (digit & flag)
            std::cout << "1";

        else
            std::cout << "0";

        flag >>= 1;
    }
}

template <class T>
std::enable_if_t< std::is_same_v<T, int>, void>
binaryRepresentation(T&& digit)//демострация пользования enable_if
{
    process<int>(std::forward<T>(digit));
}

template <class T>
std::enable_if_t< std::is_same_v<T, long long int>, void>
binaryRepresentation(T&& digit)
{
    process<long long int>(std::forward<T>(digit));
}
////////////////////////////////////////////////////////////////2.
void RemoveDups(char* str)
{
    if (str == nullptr)
        return;

    int position = 0;
    char tmp = str[position];

    for (size_t i = 1; str[i] != '\0'; i++)
    {
        if (tmp == str[i])
            ;
        else if(tmp != str[i])
        {
            str[position] = tmp;
            tmp = str[i];
            position++;
        }
    }
    str[position] = tmp;
    position++;
    str[position] = '\0';
};

///////////////////////////////////////////////////////////////3.

template<typename Type>//Для произвольного типа хранения данных
struct ListNode
{
    ListNode(Type value) : data(value) {}
    ListNode() = default;
    ListNode<Type>* prev = nullptr; // указатель на предыдущий элемент списка, либо `nullptr` в случае начала списка
    ListNode<Type>* next = nullptr;
    ListNode<Type>* rand = nullptr; // указатель на произвольный элемент данного списка, либо `nullptr`
    Type data; // произвольные пользовательские данные
};

template<typename Type>
class List
{
public:
    void Serialize(FILE* file)
    {
        if (fwrite(&(typeid(Type).name()), sizeof(size_t), 1, file));
        else throw std::exception("Cant write infoType in node");

        if(fwrite(&(count), sizeof(size_t), 1, file));
        else throw std::exception("Cant write size in node");

        for (ListNode<Type>* that = head; that != nullptr; that = that->next)
        {
            if (fwrite(&(that->data), sizeof(Type), 1, file));
            else throw std::exception("Cant write data in node");
        }
        for (ListNode<Type>* that = head; that != nullptr; that = that->next)
        {
            if (fwrite(&(that->rand), sizeof(ListNode<Type>*), 1, file));
            else throw std::exception("Cant write rand in node");
        }
    }; // сохранение списка в файл, файл открыт с помощью `fopen(path, "wb")`
    //-------------------------------------
    void Deserialize(FILE* file) 
    {
        /*char* typeInfo;
        fread(&typeInfo, sizeof(char*), 1, file);*///write typeInfo
        size_t sizeList;
        fread(&sizeList, sizeof(size_t), 1, file);
        
        if (sizeList == 0)
            return;

        Type node;
        ListNode<Type>* nodeptr;

        for (size_t i = 0; i < sizeList; i++)
        {
            if (fread(&node, sizeof(node), 1, file))
            {
                nodeptr = new ListNode<Type>(node);
                append(nodeptr);
            }
        }

        ListNode<Type>* pos;
        nodeptr = head;

        for (size_t i = 0; i < sizeList; i++)
        {
            if (fread(&pos, sizeof(ListNode<Type>*), 1, file))
            {
                nodeptr->rand = pos;
                nodeptr = nodeptr->next;
            }
        }

     }; // восстановление списка из файла, файл открыт с помощью `fopen(path, "rb")`
    //-------------------------------------
    size_t size() const noexcept { return count; }
    //-------------------------------------
    void append(ListNode<Type>* newNode) noexcept  
    {
        count++;
        if (head == nullptr)
        {
            head = newNode;
            head->rand = head;
            tail = head;
            return;
        }

        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    };
    //-------------------------------------
    void append(const Type& value)
    {
        ListNode<Type>* newNode = new ListNode<Type>{ value };
        count++;
        if (head == nullptr)
        {
            head = newNode;
            head->rand = head;
            tail = head;
            return;
        }
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;

        ListNode<Type>* newRand = head;
        size_t pos = std::rand() % size();

        while (pos--)
            newRand = newRand->next;

        newNode->rand = newRand;

    };
    //-------------------------------------
    void showList()
    {
        ListNode<Type>* tmp = head;
        while (tmp != tail)
        {
            std::cout << tmp->data <<"("<< tmp->rand->data << ")" << "-";
            tmp = tmp->next;
        }
        std::cout << tmp->data << "(" << tmp->rand->data << ")"<<"\n";
    };
    //-------------------------------------
    void clear()
    {
        ListNode<Type>* tmp = head;
        while (head->next != nullptr)
        {
            head = head->next;
            delete tmp;
            head->prev = nullptr;
            tmp = head;
        }
        count = 0;
    }
    //-------------------------------------
    ~List() { clear(); }
private:
    ListNode<Type>* head = nullptr;
    ListNode<Type>* tail = nullptr;
    size_t count = 0;
};



int main()
{
    ///////////////////////////////Test 1
    constexpr size_t lenghtInt = std::numeric_limits<int>::digits;
    constexpr size_t lenghtLInt = std::numeric_limits<long long int>::digits;
    std::bitset<lenghtInt> f(5);
    binaryRepresentation(5);
    std::cout << "\n";
    std::cout << f << "\n";
    std::cout << "-------------------------\n";
    std::bitset<lenghtLInt> f1(-5154631135);
    binaryRepresentation(-5154631135);
    std::cout << "\n";
    std::cout << f1 << "\n";
    std::cout << "-------------------------\n";

    /////////////////////////////Test 2

    char data1[] = "AAA BBB    AAA";
    RemoveDups(data1);
    assert(std::string{ data1 } == "A B A");
    std::cout << std::string{ data1 } << "\n";
    std::cout << "-------------------------\n";

    char data2[] = "A   Bccc  ---  CggGpOpOlllL";
    RemoveDups(data2);
    assert(std::string{ data2 } == "A Bc - CgGpOpOlL");
    std::cout << std::string{ data2 } <<"\n";
    std::cout << "-------------------------\n";

    char data3[] = "_555____1002..02ppp_";
    RemoveDups(data3);
    assert(std::string{ data3 } == "_5_102.02p_");
    std::cout << std::string{ data3 } << "\n";
    std::cout << "-------------------------\n";

    char data4[] = "_";
    RemoveDups(data4);
    assert(std::string{ data4 } == "_");
    std::cout << std::string{ data4 } << "\n";
    std::cout << "-------------------------\n";

    char data5[] = "!!";
    RemoveDups(data5);
    assert(std::string{ data5 } == "!");
    std::cout << std::string{ data5 } << "\n";
    std::cout << "-------------------------\n";

    char data6[] = "\\0";
    RemoveDups(data6);
    assert(std::string{ data6 } == "\\0");
    std::cout << std::string{ data6 } << "\n";
    std::cout << "-------------------------\n";

    /////////////////////////////Test 3
    const char* path = "./Test.bin";
    FILE* fin = fopen(path, "wb");
    List<std::string> l;
    l.append("1");
    l.append("2");
    l.append("3");
    l.append("40");
    l.append("5");
    l.append("6");

    std::cout << "Serialize:";
    l.showList();
    l.Serialize(fin);
    
    fclose(fin);
    std::cout << "\n";
    FILE* fout = fopen(path, "rb");
    List<int> l1;

    l1.Deserialize(fout);
    std::cout << "Deserialize:";
    l1.showList();
    fclose(fout);
}

