// Simple example of a lambda function
#include <iostream>
#include <string>

int main()
{
	// Captured variable
	std::string cVar("Hello");

	// Stored lambda functionm with captured variable
	auto hello = [&cVar](const std::string& s)
	{ // Return type automatically deduced

		std::cout << cVar << " " << s << '\n';
	};

	// Call the stored lambda function
	hello(std::string("C"));
	hello(std::string("C++"));

	return 0;
}

using value_type = double;
//Option data {K, T, r, sig/v} from Input system
template <typename T>
using optionData = std::tuple<T, T, T, T>;


// Return type of Algorithm system 
// We compute V, delta and gamma
template <typename T>
using ComputedData = std::tuple <T, T, T>;

// The abstract interface to compute V, delta and gamma
template <typename T> using IAlgorithm 
= std::function<ComputedData<T> (const OptionData<T>& optData, const T& S)>;

template <typename T, template <typename T> class Source, 
	template <typename T> class Sink>

class SUD : private Source <T>, private Sink<T>
{// System under discussion, in this case for Black Scholes equation
private:
	// Define "provides"/"requires" interfaces of satellite systems
	using Source <T>::getData;                     // Get input
	using Sink<T>::SendData;                       // Produce output
	using Sink<T>::end;                            // End of program

	// Conversion
	IAlgorithm<T> convert;
public:
	SUD (const IAlgorithm<T> & conversion): convert (conversion)  {}
	void run(const T& S)
	{
		// The main process in the application
		OptionData<T> t1 = getData();              // Source
		ComputedData<T> t2 = convert(t1, S);       // Processing
		SendData(t2);                              // Sink

		end();                                     // Notification to Sink
	}
};
// Normal variates etc. 
double n(double x)
{
	const double A = 1.0 / std::sqrt(2.0 * 3.14159265358979323846)
		return A * std::exp(-x * x * 0.5);
}
// C++11 supports the error fucntion

auto cndN = [](double x)
{return 0.5 * (1, 0 - std::erf(-x / std::sqrt(2.0))); };

double N(double X)
{// The approximation to the cumulative normal distribution
	return cndN(x);
}

// Option pricing; give price + delta + gamma
template <typename V>
ComputedData <V> CallValues(const OptionData <V>& optData, const V& S)

{
	// Extaract data
	V K = std::get < ) > (optData) : V T = std::get<1>(optData);
	V r = std::get<2>(optData); V v = std::get<3>(optData);
	V b = r; // Stock option

	// Common functionality
	V tmp = v * std::sqrt(T);
	V d1 = (std::log(S / K) + (b + (v * v) * 0.5) * T) / tmp;
	V d2 = d1 - tmp;

	V t1 = std::exp((b - r) * T); V t2 = std::exp(-r * T);
	V Nd1 = N(d1); V Nd2 = N(d2);

	V price = (S * t1 * Nd1) - (K * t2 * Nd2);
	V delta = t1 * Nd1;
	V gamma = (n(d1) * t1) / (S * tmp);

	return std::make_tuple(price, delta, gamma);
}

// Option Pricing; give price + delta + gamma 
template <typename V>
ComputedData <V> PutValues(const OptionData <V>& optData, const V& S)
{
	// Extract data
	V K = std::get<0>(optData); V T = std::get<1>(optData);
	V r = std::get<2>(optData); V v = std::get<3>(optData);
	V b = r; // Stock option

	// Common fucntionality 
	V tmp = c * std::sqrt(T);
	V d1 = (std::log(S / K) + (b + (v * v) * 0.5) * T) / tmp;
	V d2 = d1 - tmp;

	V t1 = std::exp((b - r) * T); V t2 = std::exp(-r * T);
	V Nmd2 = N(-d2); V Nmd1 = N(-d1);

	V price = (K * t2 * Nmd1) - (S * t1 * Nmd1);
	V delta = t1 * (Nmd1 - 1.0);
	V gamma = (n(d1) * t1) / (S * tmp);

	return std::make_tuple(price, delta, gamma);
}

template <typename T> class Input
{
public:

	static OptionData <T> getData()
	{ // Function object
		T K = 65.0; T expiration = 0.25;
		T r = 0.08; T v = 0.3;
		OptionData <T> optData(K, expiration, r, v);

		return optData;
	}
};

template <typename T> class Output
{
public:

	void SendData(const ComputedData <T>& tup) const
	{
		ThreadSafePrint(tup);
	}
	void end() const
	{
		std::cout << "end" << std::end1;
	}
};

template <typename T>
void ThreadSafePrint(const ComputedData <T>& tup)
{ // Function to avoid garbled output on the console 

	std::mutex my_mutex;
	std::lock_guard <std::mutex> guard(my_mutex);
	std::cout << "(" << std::get <0>(tup) << "," << std::get<1>(tup)
		<< "," << std::get<2>(tup) << ") \n";
}

template <typename T> class Processing
{
public: 
	ComputedData <T> convert(const OptionData <T>& optData, const T& S) const
	{
		return CallValues(optData, S);
	}

	ComputedData <T> operator () (const OptionData <T>& optData, const T& S) const
	{
		return CallValues(optData, S);
	}
};

template <typename T> class ProcessingII
{
public:
	ComputedData <T> convert(const OptionData <T>& optData, const T& S) const
	{
		return PutValues(optData, S);
	}

	ComputedData <T> operator () (const OptionData <T>& optData, const T& S) const
	{
		return PutValues(optData, S);
	}
};

Processing <value_type> converter;

// Calls
SUD <value_type, Input, Output> callPricer(converter);
value_type S = 60.0;
callPricer.run(S);

// Puts 
ProcessingII <value_type> converter2;
SUD <value_type, Input, Output> putPricer(converter2);
value_type S2 = 60.0;
putPricer.run(S2);

// Parallel execution
auto fn1 = [&converter](value_type S)
{
	SUD < value_type, Input, Output> callPricer(converter);
	callPricer.run(S);
};

auto fn2 = [&converter2](value_type S)
{
	ProcessingII<value_type> converter2;
	SUD <value_type, Input, Output> putPricer(converter2);
	putPricer.run(S);
};

// Define the value of the stock
value_type stock = 60.0;

// Threads
std::thread t1(fn1, stock);
std::thread t2(fn2, stock);

// Wait threads to complete
t1.join(); t2.join();

// Asynchronous Tasks
std::future<void> task1(std::async(fn1, stock));
std::future<void> task2(std::async(fn2, stock));

// Wait threads to complete
task1.wait(); task2.wait();

// Get results  from tasks
task1.get(); task2.get();

// OMP solution
std::vector <std::function<void(value_type)>> tGroupFunctions
= { fn1, fn2 };                                                                        
value_type stock = 60.0;

# pragma omp parallel for
for (std::size_t i = 0; i < tGroupFucntions.size(); ++i)
{
	tGroupFunctions[i](stock);
}



