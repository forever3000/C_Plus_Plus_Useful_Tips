#include <iostream>
#include <cstring>

class String
{
public:
    String() = default;
    String(const char* string)
    {
        printf("Created !\r\n");
        m_Size = strlen(string);
        m_Data = new char[m_Size];
        memcpy(m_Data, string, m_Size);
    }

    String(const String& other)
    {
        printf("Copied !\r\n");
        m_Size = other.m_Size;
        m_Data = new char[m_Size];
        memcpy(m_Data, other.m_Data, m_Size);
    }

    String(String&& other) noexcept
    {
        printf("Moved !\r\n");
        m_Size = other.m_Size;
        m_Data = other.m_Data;
        other.m_Size = 0;
        other.m_Data = nullptr;
    }

    void Print()
    {
        for (uint32_t i = 0; i < m_Size; i++)
        {
            printf("%c", m_Data[i]);
        }
        printf("\r\n");
    }

    ~String()
    {
        printf("Destroyed !\r\n");
        delete[] m_Data;
    }
private:
    char* m_Data;
    uint32_t m_Size;
};

class Entity
{
public:
    Entity(const String& name):
        m_Name(name)
    {

    }

    Entity(String&& name):
        m_Name(std::move(name))
    {

    }

    void PrintName()
    {
        m_Name.Print();
    }
private:
    String m_Name;
};

int main()
{
    Entity entity("ABCDEF");
    entity.PrintName();
    std::cin.get();
    return 0;
}