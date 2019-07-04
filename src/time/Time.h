#ifndef _TIME_H_
#define _TIME_H_

namespace milk
{
	class Time
	{
	public:
		virtual ~Time() = default;

		virtual double total() const = 0;
		virtual double targetFps() const = 0;
		virtual void targetFps(double target) = 0;
		virtual double fps() const = 0;
		virtual double delta() const = 0;
		virtual double scale() const = 0;
		virtual void scale(double sc) = 0;
	};
}

#endif