#ifndef __RSGE_GAME_H_
#define __RSGE_GAME_H_ 1

#include <rsge/error.h>

/**
 * \struct rsge_game_t rsge/game.h
 * \brief A structure containing game information.
 */
typedef struct {
	/**
	 * \brief The name of the game
	 */
	char* name;
} rsge_game_t;

/**
 * \fn rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo)
 * \brief A function implemented by the game, this function is called when the game engine gets the game information.
 * \param gameinfo A pointer to the gameinfo variable.
 * \return An error code.
 */
rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo);

/**
 * \fn rsge_error_e rsge_game_init()
 * \brief A function implemented by the game, this function is called when the game engine initializes or starts.
 * \return An error code.
 */
rsge_error_e rsge_game_init();

/**
 * \fn rsge_error_e rsge_game_update(double time)
 * \brief A function implemented by the game, this function is called when the game engine updates.
 * \param time Use this time for animations.
 * \return An error code.
 */
rsge_error_e rsge_game_update(double time);

/**
 * \fn rsge_error_e rsge_game_uninit()
 * \brief A function implemented by the game, this function is called when the game engine uninitializes or quits.
 * \return An error code.
 */
rsge_error_e rsge_game_uninit();

#endif
