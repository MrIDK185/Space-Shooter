#ifndef TIME_HPP
#define TIME_HPP

typedef unsigned long int __uint64_t;
typedef __uint64_t uint64_t;
typedef uint64_t Uint64;

/*------------ Clock ------------*/

struct Clock
{
public:
	//*non-static

	Uint64
		NOW = 0,
		LAST = 0;

	float deltaTimeSeconds = 0;

	void Tick();

	Clock() = default;
	~Clock() = default;
};

/*------------ SecondTimer ------------*/

struct SecondTimer
{
public:
	//*non-static

	unsigned int
		Duration = 0,
		Interval = 0;

	Uint64 nextTicks = 0;
	bool Started = false;

	void Start(Uint64 current_ticks);

	void Stop();

	bool Check(Uint64 current_ticks);

	SecondTimer() = default;
	~SecondTimer() = default;
};

#endif //! TIME_HPP
