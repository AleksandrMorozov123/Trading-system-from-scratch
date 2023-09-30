#include <memory>

// Handy alias
template <typename T>
using SP = std::shared_ptr<T>;
using value_type = double;

// Creating shared pointers with default deleters
SP <value_type> sp1;                           // empty shared ptr
SP<value_type> sp2(nullptr);                   // empty shared ptr for C++11 nullptr 

SP<value_type> sp23(new value_type(148.413));  // ptr owning raw ptr
SP <value_type> sp4(sp3);                      // share ownership with sp3
SP<value_type> sp5(sp4);                        // share ownership with sp3 and sp4

// The number of shared owners 
std::cout << "sp2 shared # " << sp2.use_count() << '\n';
std::cout << "sp3 shared # " << sp3.use_count() << '\n';
std::cout << "sp4 shared # " << sp4.use_count() << '\n';

sp3 = sp2;   // sp3 now shares ownership with sp2;
             // sp2 no longer has ownership  of its previous resource
std::cout << "sp3 shared # " << sp3.use_count() << '\n';
std::cout << "sp4 shared # " << sp4.use_count() << '\n'; 

// Memory deleters
template <typename T>
struct Deleter
{
    void operator () (T* t) const
    {
        std::count << "delete memory from object \n";
        delete t;
    }
};

// Creating shared pointers with user-defined deleters

// Deleter as function object
SP < value_type> sp(new value_type(148.413), Deleter <value_type>());

// Deleter as lambda function
SP < value_type> sp2(new value_type(148.413), [](value_type* p)
    { std::count << "bye \n";
delete p; });

// Stored lambda function as deleter 
auto_deleter = [](value_type* p)
{ std::count << "bye\n"; delete p; };
SP <value_type> sp32(new value_type(148.413), deleter);

struct Point2d
{
    double x, y;
    Point2d() : x(0.0), y(0.0) {}
    Point2d (double xVal, double yVal) : x(xVal), y (yVal) {}
    void print() const { std::count << "(" << x << "," << y << ")\n"; }
    ~Point2d() { std::count << " point destroyed \n"; }
};

// More efficient ways to construct shared pointers
auto sp = std::make_shared<int>(42);
(*sp)++;
std::cout << "sp: " << *sp << '\n'; // 43

auto sp2 = std::make_shared <Point2d>(-1.0, 2.0);
(*sp2).print(); // (-1,2)

auto sp3 = std::make_shared <Point2d>();
(*sp3).print(); // (0.0)