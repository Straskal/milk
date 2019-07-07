#ifndef _TIME_H_
#define _TIME_H_

namespace milk
{
	class Time
	{
	public:
		virtual ~Time() = default;

		virtual double total() const = 0;
		virtual void delay(double seconds) = 0;
	};
}

#endif