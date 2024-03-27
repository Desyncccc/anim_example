#pragma once
#include <functional>
 
#include "imgui.h"

  

class Animator {
public:
    using EasingFunction = std::function<float(float)>;

    // Easing
    static float EaseIn(float t) { return t * t; }
    static float EaseOut(float t) { return t * (2 - t); }
    static float EaseInOut(float t) { return t < 0.5f ? 2 * t * t : -1 + (4 - 2 * t) * t; }
    static float SmoothStep(float t) { return t * t * (3 - 2 * t); }
    static float SmootherStep(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    static float FadeIn(float t) { return t; }
    static float FadeOut(float t) { return 1 - t; }
    static float SmoothTransition(float t) { return t * t * (3 - 2 * t); }

    // Utility
    template <typename T>
    static T Clamp(T v, T min, T max) {
        return (v < min) ? min : (v > max) ? max : v;
    }

    static float Min(float a, float b) {
        return (a < b) ? a : b;
    }

    static float Max(float a, float b) {
        return (a > b) ? a : b;
    }

   
    static float Animate(float time, float duration, EasingFunction easing, float delay = 0.0f, bool repeat = false, bool yoyo = false) {
        if (time < delay) {
            return 0.0f;  
        }

        float adjustedTime = time - delay;
        float t = Clamp(adjustedTime / duration, 0.0f, 1.0f);

       
        float animationProgress = easing(t);

        if (repeat) {
            int cycle = static_cast<int>(adjustedTime / duration);
            if (yoyo && (cycle % 2 != 0)) {
             
                animationProgress = 1.0f - animationProgress;
            }
        }

        return animationProgress;
    }

     
   
};

namespace custom {
    bool Checkbox(const char* label, bool* v);
    bool CheckboxFade(const char* label, bool* v);

}
