#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SDL2/SDL.h>

struct Game;

class EventHandler
{
private:
	//* non-static

	SDL_Event Event;
	Game *currentGame;

	bool Handle_WindowQuit();

	bool Handle_Q();
	bool Handle_Escape();
	bool Handle_Return();
	bool Handle_F11();
	bool Handle_KP_Plus();
	bool Handle_KP_Minus();
	bool Handle_M();

	bool Handle_TimerEvent();
	bool Handle_CountdownDecrement();
	bool Handle_GameStart();
	bool Handle_GemBlink();
	bool Handle_GemDisappear();
	bool Handle_PlayerEffectStop();

	void HandleGameStarted();
	void HandleGamePaused();

public:
	//* non-static

	explicit EventHandler();
	explicit EventHandler(Game *current_game);
	~EventHandler();

	void HandleEvents();
};

#endif //! EVENT_HANDLER_HPP