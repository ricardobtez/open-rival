#include "pch.h"
#include "Application.h"

#include <SDL.h>

namespace Rival {

    Application::Application(Window& window)
        : window(window) {

        // Try to enable vsync.
        // Note that vsync may already be enabled by default!
        if (SDL_GL_SetSwapInterval(1) == 0) {
            vsyncEnabled = true;
        } else {
            printf("Unable to enable vsync! SDL Error: %s\n", SDL_GetError());
            vsyncEnabled = false;
        }
    }

    void Application::start(std::unique_ptr<State> initialState) {

        state = std::move(initialState);
        Uint32 nextUpdateDue = SDL_GetTicks();

        // Game loop
        while (!exiting) {

            // Determine when this frame began.
            // If we are running behind, this will be a long time after
            // nextUpdateDue, so we will have to update the logic multiple
            // times to catch up. If we are ahead of schedule, this will be
            // BEFORE nextUpdateDue, and we will need to wait a little longer.
            Uint32 frameStartTime = SDL_GetTicks();

            // Is the next update due?
            if (vsyncEnabled || nextUpdateDue <= frameStartTime) {

                // Handle events on the queue
                pollEvents();

                // Update the game logic, as many times as necessary to keep it
                // in-sync with the refresh rate.
                //
                // For example:
                //  - For a 30Hz monitor, this will run twice per render.
                //  - For a 60Hz monitor, this will run once per render.
                //  - For a 120Hz monitor, this will run every other render.
                //
                // If vsync is disabled, this should run once per render.
                while (nextUpdateDue <= frameStartTime) {
                    state->update();
                    nextUpdateDue += TimerUtils::timeStepMs;
                }

                // Render the game, once per iteration.
                // With vsync enabled, this matches the screen's refresh rate.
                // Otherwise, this matches our target FPS.
                state->render();

                // Update the window with our newly-rendered game.
                // If vsync is enabled, this will block execution until the
                // next swap interval.
                window.swapBuffers();

            } else {
                // Next update is not yet due (frameStartTime < nextUpdateDue),
                // so let's sleep (unless the next update is imminent!)
                Uint32 sleepTime = nextUpdateDue - frameStartTime;
                if (sleepTime >= minSleepTime) {
                    SDL_Delay(sleepTime);
                }
            }
        }
    }

    void Application::pollEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                exiting = true;
            } else if (e.type == SDL_KEYDOWN) {
                state->keyDown(e.key.keysym.sym);
            } else if (e.type == SDL_MOUSEWHEEL) {
                state->mouseWheelMoved(e.wheel);
            }
        }
    }

    const Window& Application::getWindow() const {
        return window;
    }

    Resources& Application::getResources() {
        return res;
    }

}  // namespace Rival