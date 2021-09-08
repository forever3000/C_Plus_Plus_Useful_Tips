#include <iostream>
#include <memory>

// #define UNIQUE_PTR
// #define SHARED_PTR
#define WEAK_PTR

class Entity
{
public:
    Entity()
    {
        std::cout << "Created Entity!" << std::endl;
    }

    ~Entity()
    {
        std::cout << "Destroyed Entity!" << std::endl;
    }

    void Print() {};
};

#ifdef UNIQUE_PTR
int main()
{
    {
        // std::unique_ptr<Entity> entity(new Entity());
        std::unique_ptr<Entity> entity = std::make_unique<Entity>();
        entity->Print();

        // std::unique_ptr<Entity> e0 = entity; // Can not copy with unique pointer
    }

    std::cout << "Finish App!!!" << std::endl;

    return 0;
}
#endif //UNIQUE_PTR

#ifdef SHARED_PTR
int main()
{
    {
        std::shared_ptr<Entity> e0;
        {
            std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>();
            e0 = sharedEntity;
            e0->Print();
        }
        std::cout << "Finish Scope 1!!!" << std::endl;
    }

    std::cout << "Finish Scope 2!!!" << std::endl;

    return 0;
}
#endif

#ifdef WEAK_PTR
int main()
{
    {
        std::weak_ptr<Entity> e0;
        {
            std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>();
            e0 = sharedEntity;
            // e0->Print(); weak pointer is not a pointer 
        }
        
        std::cout << "Finish Scope 1!!!" << std::endl;
    }

    std::cout << "Finish Scope 2!!!" << std::endl;

    return 0;
}
#endif