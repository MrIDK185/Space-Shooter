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

	void Handle_WindowQuit() const;

	void Handle_Q() const;
	void Handle_Escape() const;
	void Handle_Return() const;
	void Handle_F11() const;
	void Handle_KP_Plus() const;
	void Handle_KP_Minus() const;
	void Handle_M() const;

	void Handle_TimerEvent() const;
	void Handle_CountdownDecrement() const;
	void Handle_GameStart() const;
	void Handle_GemBlink() const;
	void Handle_GemDisappear() const;
	void Handle_PlayerEffectStop() const;

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