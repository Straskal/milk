#ifndef MILK_TIMER_H
#define MILK_TIMER_H

namespace milk
{
    // TODO: This class needs to be abstracted away. This contains the SDL implementation.
    /// A Timer!
    class Timer
    {
    public:
        Timer();

        /// Start the Timer
        void start();

        /// Stop the Timer
        void stop();

        /// Pause the Timer
        void pause();

        /// Unpause the Timer
        void unpause();

        /// \returns the time since the Timer has been started in milliseconds
        int milliseconds();

        /// \returns the time since the Timer has been started in seconds
        float seconds();

        /// \returns true if the Timer had been started
        bool isStarted();

        /// \returns true if the Timer is paused
        bool isPaused();

    private:
        int startTicks_;
        int pausedTicks_;

        bool paused_;
        bool started_;
    };
}

#endif
