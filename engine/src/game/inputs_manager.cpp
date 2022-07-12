#include <SDL.h>

#include <backends/imgui_impl_sdl.h>

#include "game/inputs_manager.hpp"

using namespace Game;

#pragma region STATIC DEFINITION
bool Inputs::quit = false;
bool Inputs::allowListeningInputs = false;

std::unordered_map<SDL_KeyCode, EButton> Inputs::keys {};
std::unordered_map<SDL_GameControllerAxis , EButton> Inputs::positiveJoysticks {};
std::unordered_map<SDL_GameControllerAxis , EButton> Inputs::negativeJoysticks {};

std::unordered_map<std::string, ButtonAction> Inputs::buttonActions {};
std::unordered_map<std::string, AxisAction> Inputs::axisActions {};
std::list<EButtonState*> Inputs::keyPressed {};
MouseAction Inputs::mouse {};
JoystickAction Inputs::joysticks {};
SDL_GameController* Inputs::gamepad = nullptr;

EnumArray<EButton, EButtonState, static_cast<size_t>(EButton::BUTTON_COUNT)> Inputs::buttonStates {};
#pragma endregion

void Inputs::KeyPressed(EButtonState& o_state)
{
    if (o_state == EButtonState::RELEASED)
    {
        o_state = EButtonState::PRESSED;
        keyPressed.emplace_back(&o_state);
    }
}

void Inputs::CheckKeyPressed()
{
    auto it = keyPressed.begin();
    while(it != keyPressed.end())
    {
        if (**it == EButtonState::PRESSED) **it = EButtonState::DOWN;
        it = keyPressed.erase(it);
    }
}

EButtonState* Inputs::GetButtonState(Game::EButton i_button)
{
    return &buttonStates[i_button];
}

void Game::Inputs::SetButtonAction(const std::string& i_name, const std::initializer_list<EButton>& i_inputs, bool i_resetKeys)
{
    if(buttonActions.find(i_name) == buttonActions.end())
    {
        buttonActions[i_name] = ButtonAction{};
        Log::Info(std::string("New button action map created : '") + i_name + std::string("'"));
    }

    ButtonAction& action = buttonActions[i_name];

    if (i_resetKeys) action.buttons.clear();

    for (EButton key : i_inputs)
        action.buttons.emplace_back(GetButtonState(key));
}

void Inputs::SetAxisAction(const std::string& i_name, const std::initializer_list<EButton>& i_positiveInputs, const std::initializer_list<EButton>& i_negativeInputs, bool i_resetPositive, bool i_resetNegative)
{
    if(axisActions.find(i_name) == axisActions.end())
    {
        axisActions[i_name] = AxisAction{};
        Log::Info(std::string("New axis action map created : '") + i_name + std::string("'"));
    }

    AxisAction& action = axisActions[i_name];

    if (i_resetPositive) action.positiveButtons.clear();

    for (EButton key : i_positiveInputs)
        action.positiveButtons.emplace_back(GetButtonState(key));

    if (i_resetNegative) action.negativeButtons.clear();

    for (EButton key : i_negativeInputs)
        action.negativeButtons.emplace_back(GetButtonState(key));
}

Inputs::Inputs()
{
    keys[SDLK_a] = EButton::A;
    keys[SDLK_b] = EButton::B;
    keys[SDLK_c] = EButton::C;
    keys[SDLK_d] = EButton::D;
    keys[SDLK_e] = EButton::E;
    keys[SDLK_f] = EButton::F;
    keys[SDLK_g] = EButton::G;
    keys[SDLK_h] = EButton::H;
    keys[SDLK_i] = EButton::I;
    keys[SDLK_j] = EButton::J;
    keys[SDLK_k] = EButton::K;
    keys[SDLK_l] = EButton::L;
    keys[SDLK_m] = EButton::M;
    keys[SDLK_n] = EButton::N;
    keys[SDLK_o] = EButton::O;
    keys[SDLK_p] = EButton::P;
    keys[SDLK_q] = EButton::Q;
    keys[SDLK_r] = EButton::R;
    keys[SDLK_s] = EButton::S;
    keys[SDLK_t] = EButton::T;
    keys[SDLK_u] = EButton::U;
    keys[SDLK_v] = EButton::V;
    keys[SDLK_w] = EButton::W;
    keys[SDLK_x] = EButton::X;
    keys[SDLK_y] = EButton::Y;
    keys[SDLK_z] = EButton::Z;
    keys[SDLK_LSHIFT] = EButton::LEFT_SHIFT;
    keys[SDLK_RSHIFT] = EButton::RIGHT_SHIFT;
    keys[SDLK_LCTRL] = EButton::LEFT_CTRL;
    keys[SDLK_RCTRL] = EButton::RIGHT_CTRL;
    keys[SDLK_LALT] = EButton::ALT;
    keys[SDLK_SPACE] = EButton::SPACE;
    keys[SDLK_DOWN] = EButton::ARROW_DOWN;
    keys[SDLK_UP] = EButton::ARROW_UP;
    keys[SDLK_RIGHT] = EButton::ARROW_RIGHT;
    keys[SDLK_LEFT] = EButton::ARROW_LEFT;
    keys[SDLK_ESCAPE] = EButton::ESCAPE;
    keys[SDLK_F1] = EButton::F1;
    keys[SDLK_F2] = EButton::F2;
    keys[SDLK_F3] = EButton::F3;
    keys[SDLK_F4] = EButton::F4;
    keys[SDLK_F5] = EButton::F5;
    keys[SDLK_F6] = EButton::F6;
    keys[SDLK_F7] = EButton::F7;
    keys[SDLK_F8] = EButton::F8;
    keys[SDLK_F9] = EButton::F9;
    keys[SDLK_F10] = EButton::F10;
    keys[SDLK_F11] = EButton::F11;
    keys[SDLK_F12] = EButton::F12;

    positiveJoysticks[SDL_CONTROLLER_AXIS_LEFTX] = EButton::LEFT_JOYSTICK_RIGHT;
    positiveJoysticks[SDL_CONTROLLER_AXIS_LEFTY] = EButton::LEFT_JOYSTICK_DOWN;
    positiveJoysticks[SDL_CONTROLLER_AXIS_RIGHTX] = EButton::RIGHT_JOYSTICK_RIGHT;
    positiveJoysticks[SDL_CONTROLLER_AXIS_RIGHTY] = EButton::RIGHT_JOYSTICK_DOWN;

    negativeJoysticks[SDL_CONTROLLER_AXIS_LEFTX] = EButton::LEFT_JOYSTICK_LEFT;
    negativeJoysticks[SDL_CONTROLLER_AXIS_LEFTY] = EButton::LEFT_JOYSTICK_UP;
    negativeJoysticks[SDL_CONTROLLER_AXIS_RIGHTX] = EButton::RIGHT_JOYSTICK_LEFT;
    negativeJoysticks[SDL_CONTROLLER_AXIS_RIGHTY] = EButton::RIGHT_JOYSTICK_UP;
}

void Inputs::PollEvent(const InputsEvent& editorEvent)
{
    SDL_Event event;
    CheckKeyPressed();

    /// MOUSE UDPATE
    int x, y, relX, relY;
    SDL_GetMouseState(&x, &y);
    SDL_GetRelativeMouseState(&relX, &relY);
    mouse.position = Vector2(x, y);
    mouse.deltaMotion = Vector2(relX, relY);

    /// GAMEPAD JOYSTICKS RESET

    if (gamepad)
        UpdateJoysticksDirection();

    /// OPEN GAMEPAD
    if (!gamepad)
    {
        for (int i = 0; i < SDL_NumJoysticks(); i++)
        {
            if (SDL_IsGameController(i))
            {
                gamepad = SDL_GameControllerOpen(i);
                break;
            }
        }
    }


    EButton button;
    SDL_GameControllerAxis axis;
    /// INPUTS EVENT
    while (SDL_PollEvent(&event))
    {
        if (editorEvent) editorEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                Inputs::quit = true;
                break;
            case SDL_KEYDOWN:
                button = keys[static_cast<SDL_KeyCode>(event.key.keysym.sym)];
                KeyPressed(buttonStates[button]);
                break;
            case SDL_KEYUP:
                button = keys[static_cast<SDL_KeyCode>(event.key.keysym.sym)];
                buttonStates[button] = EButtonState::RELEASED;
                break;

            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        KeyPressed(buttonStates[EButton::MOUSE_LEFT]); break;
                    case SDL_BUTTON_MIDDLE:
                        KeyPressed(buttonStates[EButton::MOUSE_MIDDLE]); break;
                    case SDL_BUTTON_RIGHT:
                        KeyPressed(buttonStates[EButton::MOUSE_RIGHT]); break;
                    default: break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT: buttonStates[EButton::MOUSE_LEFT] = EButtonState::RELEASED; break;
                    case SDL_BUTTON_MIDDLE: buttonStates[EButton::MOUSE_MIDDLE] = EButtonState::RELEASED; break;
                    case SDL_BUTTON_RIGHT:  buttonStates[EButton::MOUSE_RIGHT] = EButtonState::RELEASED; break;
                    default: break;
                }
                break;

            case SDL_CONTROLLERBUTTONDOWN:
                button = static_cast<EButton>(event.cbutton.button + static_cast<Uint8>(EButton::GAMEPAD_A));
                KeyPressed(buttonStates[button]);
                break;
            case SDL_CONTROLLERBUTTONUP:
                button = static_cast<EButton>(event.cbutton.button + static_cast<Uint8>(EButton::GAMEPAD_A));
                buttonStates[button] = EButtonState::RELEASED;
                break;
            case SDL_CONTROLLERAXISMOTION:
                axis = static_cast<SDL_GameControllerAxis>(event.caxis.axis);

                if (event.caxis.value < 0)
                    button = negativeJoysticks[axis];
                else
                    button = positiveJoysticks[axis];

                if (event.caxis.value < -20000 || event.caxis.value > 20000)
                    KeyPressed(buttonStates[button]);
                else
                    buttonStates[button] = EButtonState::RELEASED;

                break;
            default:
                break;
        }
    }

    quit |= IsAlterateCommandPressed(EButton::F4);
}

#pragma region Action Getter
EButtonState Inputs::GetButton(const std::string& i_name)
{
    if (CheckButtonAction(i_name))
        return buttonActions[i_name].state;
    else
        return EButtonState::RELEASED;
}

bool Inputs::IsPressed(const std::string& i_name)
{
    if (CheckButtonAction(i_name))
        return buttonActions[i_name].state == EButtonState::PRESSED;
    else
        return false;
}

bool Inputs::IsPressed(EButton i_button)
{
    return ApplyListeningCheck(buttonStates[i_button] == EButtonState::PRESSED);
}

bool Inputs::IsDown(const std::string& i_name)
{
    if (CheckButtonAction(i_name))
        return buttonActions[i_name].state == EButtonState::DOWN;
    else
        return false;
}

bool Inputs::IsDown(EButton i_button)
{
    return ApplyListeningCheck(buttonStates[i_button] == EButtonState::DOWN);
}

bool Inputs::IsReleased(const std::string& i_name)
{
    if (CheckButtonAction(i_name))
        return buttonActions[i_name].state == EButtonState::RELEASED;
    else
        return false;
}

bool Inputs::IsReleased(EButton i_button)
{
    return ApplyListeningCheck(buttonStates[i_button] == EButtonState::RELEASED);
}

bool Game::Inputs::IsControlCommandPressed(EButton i_button)
{
    return ApplyListeningCheck((buttonStates[EButton::LEFT_CTRL] != EButtonState::RELEASED
            || buttonStates[EButton::RIGHT_CTRL] != EButtonState::RELEASED)
           && buttonStates[i_button] == EButtonState::PRESSED);
}

bool Game::Inputs::IsAlterateCommandPressed(EButton i_button)
{
    return buttonStates[EButton::ALT] != EButtonState::RELEASED
          && buttonStates[i_button] == EButtonState::PRESSED;
}

float Inputs::GetAxis(const std::string& i_name)
{
    if (!CheckAxisAction(i_name)) return 0.f;

    return axisActions[i_name].value;
}

Vector2 Inputs::GetMousePosition()
{
    if (ApplyListeningCheck())
        return mouse.position;
    else
        return {};
}

Vector2 Inputs::GetMouseDelta()
{
    if (ApplyListeningCheck())
        return mouse.deltaMotion;
    else
        return {};
}

Vector2 Inputs::GetLeftJoystickDirection()
{
    if (ApplyListeningCheck())
    {
        joysticks.leftJoystick = Vector3::ClampLength(joysticks.leftJoystick, 0.f, 1.f);
        return joysticks.leftJoystick;
    }
    else
        return {};
}

Vector2 Inputs::GetRightJoystickDirection()
{
    if (ApplyListeningCheck())
    {
    joysticks.leftJoystick = Vector3::ClampLength(joysticks.rightJoystick, 0.f, 1.f);
    return joysticks.rightJoystick;
    }
    else
        return {};
}

#pragma endregion

bool Inputs::RumbleGamepad(float i_time, float i_power)
{
    i_power = Maths::Clamp(i_power, 0.f, 1.f);
    if (gamepad)
    {
        auto milliDuration = static_cast<Uint32>(i_time * 1000.f);
        auto sdlPower = static_cast<Uint16>(i_power * static_cast<float>(std::numeric_limits<Uint16>::max()));
        if (SDL_GameControllerRumble(gamepad, sdlPower, sdlPower, milliDuration))
            return true;
    }
    return false;
}

bool Inputs::RumbleGamepadPro(float i_time, float i_leftPower, float i_rightPower)
{
    i_leftPower = Maths::Clamp(i_leftPower, 0.f, 1.f);
    i_rightPower = Maths::Clamp(i_rightPower, 0.f, 1.f);

    if (gamepad)
    {
        // Cast
        auto milliDuration = static_cast<Uint32>(i_time * 1000.f);
        auto sdlLeftPower = static_cast<Uint16>(i_leftPower * static_cast<float>(std::numeric_limits<Uint16>::max()));
        auto sdlRightPower = static_cast<Uint16>(i_rightPower * static_cast<float>(std::numeric_limits<Uint16>::max()));
        // Rumble
        if (SDL_GameControllerRumble(gamepad, sdlLeftPower, sdlRightPower, milliDuration))
            return true;
    }
    return false;
}

bool Game::Inputs::CheckButtonAction(const std::string& i_name)
{
    // FIND
    if (buttonActions.find(i_name) == buttonActions.end())
    {
        Log::Warning(std::string("The button action '") + i_name + std::string("' does not exist."));
        return false;
    }

    ButtonAction& action = buttonActions[i_name];
    action.state = EButtonState::RELEASED;
    for(EButtonState* state : action.buttons)
    {
        if (*state == EButtonState::DOWN)
        {
            action.state = EButtonState::DOWN;
            return true;
        }
        else if (*state == EButtonState::PRESSED)
            action.state = EButtonState::PRESSED;
    }

    return ApplyListeningCheck(true);
}

bool Game::Inputs::CheckAxisAction(const std::string& i_name)
{
    // FIND
    if (axisActions.find(i_name) == axisActions.end())
    {
        Log::Warning(std::string("The axis action '") + i_name + std::string("' does not exist."));
        return false;
    }

    AxisAction& action = axisActions[i_name];
    action.value = 0.f;
    for (const EButtonState* state : action.positiveButtons)
    {
        if (*state != EButtonState::RELEASED)
        {
            action.value += 1.f;
            break;
        }
    }
    for (const EButtonState* state : action.negativeButtons)
    {
        if (*state != EButtonState::RELEASED)
        {
            action.value -= 1.f;
            break;
        }
    }
    return ApplyListeningCheck(true);
}

void Inputs::SetInputsListening(bool i_listening) noexcept
{
    allowListeningInputs = i_listening;
}

bool Inputs::ApplyListeningCheck(bool i_currentValue)
{
    return allowListeningInputs && i_currentValue;
}

void Inputs::UpdateJoysticksDirection()
{
    joysticks.leftJoystick = Vector2();
    joysticks.rightJoystick = Vector2();

    float normalizedAxisValue = static_cast<float>(SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_LEFTX)) / SDLAxisMaxValue;
    joysticks.leftJoystick.x = Maths::Abs(normalizedAxisValue) < joystickAxisMinimumValue ? 0.f : normalizedAxisValue;

    normalizedAxisValue = static_cast<float>(SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_LEFTY)) / SDLAxisMaxValue;
    joysticks.leftJoystick.y = Maths::Abs(normalizedAxisValue) < joystickAxisMinimumValue ? 0.f : -normalizedAxisValue;

    normalizedAxisValue = static_cast<float>(SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_RIGHTX)) / SDLAxisMaxValue;
    joysticks.rightJoystick.x = Maths::Abs(normalizedAxisValue) < joystickAxisMinimumValue ? 0.f : normalizedAxisValue;

    normalizedAxisValue = static_cast<float>(SDL_GameControllerGetAxis(gamepad, SDL_CONTROLLER_AXIS_RIGHTY)) / SDLAxisMaxValue;
    joysticks.rightJoystick.y = Maths::Abs(normalizedAxisValue) < joystickAxisMinimumValue ? 0.f : -normalizedAxisValue;
}
