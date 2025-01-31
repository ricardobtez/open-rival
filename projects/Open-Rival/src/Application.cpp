#include "pch.h"

#include "Application.h"

#include "SDLWrapper.h"

#include <memory>
#include <utility>  // std::in_place, std::move

#include "net/Socket.h"
#include "ApplicationContext.h"
#include "ConfigUtils.h"
#include "PlayerState.h"
#include "State.h"
#include "TimeUtils.h"

namespace Rival {

Application::Application(ApplicationContext& context)
    : context(context)
{
}

void Application::start(std::unique_ptr<State> initialState)
{
    setState(std::move(initialState));

    Window* window = context.getWindow();
    const bool vsyncEnabled = window->isVsyncEnabled();

    TimeUtils::PrecisionTimer timer;

    // Game loop
    Uint32 nextUpdateDue = SDL_GetTicks();
    while (!exiting)
    {
        // Switch to the next State, if set
        if (nextState.get())
        {
            makeNextStateActive();
        }

        // Determine when this frame began.
        // If we are running behind, this will be a long time after
        // nextUpdateDue, so we will have to update the logic multiple
        // times to catch up. If we are ahead of schedule, this will be
        // BEFORE nextUpdateDue, and we will need to wait a little longer.
        Uint32 frameStartTime = SDL_GetTicks();

        // Is the next update due?
        if (vsyncEnabled || nextUpdateDue <= frameStartTime)
        {
            // Handle events on the queue
            pollEvents();

            // Update the game logic as many times as necessary to keep it
            // up to date with the current time. The idea is to run the
            // logic at a fixed rate, regardless of the game's framerate.
            //
            // For example:
            //  - For a 30Hz monitor, this will run twice per render.
            //  - For a 60Hz monitor, this will run once per render.
            //  - For a 120Hz monitor, this will run every other render.
            //
            // If vsync is disabled, this should run once per render.
            while (nextUpdateDue <= frameStartTime)
            {
                state->update();
                nextUpdateDue += TimeUtils::timeStepMs;
            }

            // Calculate our delta time for rendering. This is the number of
            // milliseconds that have passed since the game logic was last
            // updated. This enables us to accurately extrapolate unit
            // positions between logical movements.
            Uint32 now = SDL_GetTicks();
            Uint32 lastUpdate = nextUpdateDue - TimeUtils::timeStepMs;
            Uint32 delta = now - lastUpdate;

            // Render the game, once per iteration. With vsync enabled, this
            // will be called at the same frequency as the screen's refresh
            // rate. Otherwise, this will be called at a frequency that
            // matches our target FPS.
            state->render(static_cast<int>(delta));

            // Update the window with our newly-rendered game.
            // If vsync is enabled, this will block execution until the
            // next swap interval.
            window->swapBuffers();
        }
        else
        {
            // Next update is not yet due (frameStartTime < nextUpdateDue),
            // so let's sleep (unless the next update is imminent!)
            Uint32 sleepTime = nextUpdateDue - frameStartTime;
            if (sleepTime >= TimeUtils::minSleepTime)
            {
                timer.wait(sleepTime);
            }
        }
    }
}

void Application::pollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            exiting = true;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            state->keyDown(e.key.keysym.sym);
        }
        else if (e.type == SDL_KEYUP)
        {
            state->keyUp(e.key.keysym.sym);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            state->mouseDown(e.button);
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            state->mouseUp(e.button);
        }
        else if (e.type == SDL_MOUSEWHEEL)
        {
            state->mouseWheelMoved(e.wheel);
        }
    }
}

void Application::makeNextStateActive()
{
    state = std::move(nextState);
    state->onLoad();
}

void Application::startServer(std::uint16_t port)
{
    std::cout << "Starting server on port " << std::to_string(port) << "\n";

    server.emplace(port, PlayerStore::maxPlayers);
    server->start();

    // Connect to the server ourselves!
    connectToServer("localhost", port);
}

void Application::connectToServer(const std::string& address, std::uint16_t port)
{
    std::cout << "Connecting to server on port " << std::to_string(port) << "\n";

    if (!packetFactory)
    {
        packetFactory = std::make_shared<PacketFactory>();
    }

    Socket clientSocket = Socket::createClient(address, port);
    connection.emplace(std::move(clientSocket), packetFactory);
}

}  // namespace Rival
