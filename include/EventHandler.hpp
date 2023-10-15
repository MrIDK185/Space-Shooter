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

public:
	explicit EventHandler(Game *current_game);
	~EventHandler();

	void HandleEvents();
};

#endif //! EVENT_HANDLER_HPP