#pragma warning(disable : 4996)
///test only
#include <bitset>
#include <cassert>
/////////////////
#include <type_traits>
#include <iostream>
#include <io.h>
#include <map>
#include <unordered_map>
#include <string_view>
#include <limits>
#define _CRT_NONSTDC_NO_WARNINGS
///////////////////////////////////////////////////////////////1.
template <typename T>
void process(const T&& digit) 
{
    T flag = std::numeric_limits<T>::max() / 2 + 1;//получение старшего бита

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
    //для каждой позиции выбирается временной элемент и сравнивается с последующими
    str[position] = tmp;
    position++;
    str[position] = '\0';//"обрезание" исходной строки
};

///////////////////////////////////////////////////////////////3.
struct ListNode
{
    ListNode(std::string value) : data(value) {}
    ListNode() = default;
    ListNode* prev = nullptr; // указатель на предыдущий элемент списка, либо `nullptr` в случае начала списка
    ListNode* next = nullptr;
    ListNode* rand = nullptr; // указатель на произвольный элемент данного списка, либо `nullptr`
    std::string data; // произвольные пользовательские данные
};

class List
{
public:
    void Serialize(FILE* file)
    {
        std::map<ListNode*, unsigned> nodeKey;
        unsigned ind = 0;


        for (ListNode* curr = head;
            curr != nullptr;
            curr = curr->next, ind++)
        nodeKey.insert(std::pair<ListNode*, unsigned> (curr, ind) );
        //заполнение для узла его id

        if(fwrite(&(count), sizeof(size_t), 1, file));
        else throw std::exception{ "Cant write size of nodes." };


        unsigned i = 0;
        for (ListNode* curr = head; curr != nullptr; curr = curr->next)
        {
            if (fwrite(curr->data.c_str(), sizeof(char*), 1, file));
            else throw std::exception{ "Cant write data in node." };

            i = nodeKey[curr->rand];//нахождение по рандомному узлу его id и запись в фаил
            if (fwrite(&(i), sizeof(unsigned), 1, file));
            else throw std::exception{ "Cant write rand data in node." };
        }
        //запись узлов


    }; // сохранение списка в файл, файл открыт с помощью `fopen(path, "wb")`
    //-------------------------------------
    void Deserialize(FILE* file) 
    {
        clear();

        size_t sizeList = 0;
        fread(&sizeList, sizeof(size_t), 1, file);
        
        if (sizeList == 0)
            throw std::exception{"Zero size."};


        char* node;
        ListNode* nodeptr;
        unsigned nodeNumber;

        std::vector<ListNode*> nodes;
        std::vector<unsigned> indxRand;

        nodes.reserve(sizeList);
        indxRand.reserve(sizeList);

        for (size_t i = 0; i < sizeList; i++)
        {
            if (fread(&node, sizeof(char*), 1, file))
            {
                append(node);
                nodes.push_back(tail);
            }
            else
            {
                append(nullptr);
                nodes.push_back(nullptr);
            }//заполнение векторов и листа

            if (fread(&nodeNumber, sizeof(unsigned), 1, file))
                indxRand.push_back(nodeNumber);

            else
                indxRand.push_back(0);
        }//запись узлов

        ListNode* pos;
        nodeptr = head;
        unsigned ind = 0;

        for (ListNode* curr = head;
            curr != nullptr;
            curr = curr->next, ind++)
        curr->rand = nodes[indxRand[ind]];//по номеру из numsOfRand присваивается значение
        //запись рандомных членов

     }; // восстановление списка из файла, файл открыт с помощью `fopen(path, "rb")`
    //-------------------------------------
    size_t size() const noexcept { return count; }
    //-------------------------------------
    void append(ListNode* newNode) noexcept  
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
    void append(std::string value)
    {
        ListNode* newNode = new ListNode{ value.data() };
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
    void setRandNodes() 
    {
        ListNode* rand = head;


        for (ListNode* curr = head;
            curr != nullptr;
            curr = curr->next)
        {
            size_t pos = std::rand() % size();
            while (pos--)
                rand = rand->next;
            curr->rand = rand;
            rand = head;
        }

        
    }
    //-------------------------------------
    void showList()
    {
        ListNode* tmp = head;
        while (tmp != tail)
        {
            std::cout << tmp->data <<"("<< tmp->rand->data << ")";
            tmp = tmp->next;
        }
        std::cout << tmp->data << "(" << tmp->rand->data << ")"<<"\n";
    };
    //-------------------------------------
    void clear()
    {
        if (head == nullptr)
            return;

        ListNode* tmp = head;
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
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
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
    List l;
    l.append("Hello");
    l.append(", ");
    l.append("my ");
    l.append("name ");
    l.append("is ");
    l.append("Vladislav");
    l.setRandNodes();

    std::cout << "Serialize:";
    l.showList();

    try
    {

        l.Serialize(fin);
    
        fclose(fin);
        std::cout << "\n";
        FILE* fout = fopen(path, "rb");
        List l1;

        l1.Deserialize(fout);
        std::cout << "Deserialize:";
        l1.showList();

        fclose(fout);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what()<<"\n";
    }

}

////Уразлин Владислав Сергеевич
//10.01.2023, 6 часов