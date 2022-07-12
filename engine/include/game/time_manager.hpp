#pragma once

namespace Game
{
    class Time
    {
    public:
        /**
         * Update time and deltaTime value
         */
        static void NewFrame();

        /**
         * @return the program execution time in seconds
         */
        static float GetTime();

        /**
         * @return the current frame time, affected by the time scale
         */
        static float GetDeltaTime();

        /**
         * @return the real frame time, not affected by the time scale
         */
        static float GetRawDeltaTime();

        /**
         * @return the fixed frame time, affected by the time scale
         */
        static float GetFixedDeltaTime();

        /**
         * @return the fixed frame time, not affected by the time scale
         */
        static float GetRawFixedDeltaTime();

        /**
         * @return the time scale factor that is applied on delta time to control frame speed.
         */
        static float GetTimeScale();

        /**
         * @brief set the time scale factor, should stay 1 to have regular speed but you can make slow/fast motion or even pause the game.
         */
        static void SetTimeScale(float i_scale);

    private:
        static float time;
        static float previousTime;
        static float deltaTime;
        static float timeScale;
        static constexpr float fixedDeltaTime = 0.016f;
    };
}
