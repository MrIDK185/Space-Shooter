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

	void Handle_WindowQuit();

	void Handle_Q();
	void Handle_Escape();
	void Handle_Return();
	void Handle_F11();
	void Handle_KP_Plus();
	void Handle_KP_Minus();
	void Handle_M();

	void Handle_TimerEvent();
	void Handle_CountdownDecrement();
	void Handle_GameStart();
	void Handle_GemBlink();
	void Handle_GemDisappear();
	void Handle_PlayerEffectStop();

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