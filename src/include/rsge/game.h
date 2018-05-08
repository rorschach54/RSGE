#ifndef __RSGE_GAME_H_
#define __RSGE_GAME_H_ 1

/** \file src/include/rsge/game.h
 * \brief A header file to be used for the game itself.
 * \author Spaceboy Ross
 */

#include <rsge/gfx/elglr.h>
#include <rsge/error.h>
#include <rsge/input.h>

/**
 * \struct rsge_game_t rsge/game.h
 * \brief A structure containing game information.
 */
typedef struct {
	/**
	 * \brief The name of the game
	 */
	char* name;
	
	/**
	 * \brief The id of the game
	 */
	char* id;
} rsge_game_t;

/**
 * \fn rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo)
 * \brief A function implemented by the game, this function is called when the game engine gets the game information.
 * \param[out] gameinfo A pointer to the gameinfo variable.
 * \return An error code.
 */
rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo);

/**
 * \fn rsge_error_e rsge_game_init(rsge_elglr_t* elglr)
 * \brief A function implemented by the game, this function is called when the game engine initializes or starts.
 * \param[in] elglr The ELGLR instance used.
 * \return An error code.
 */
rsge_error_e rsge_game_init(rsge_elglr_t* elglr);

/**
 * \fn rsge_error_e rsge_game_update(rsge_elglr_t* elglr,double currentTime,int fps)
 * \brief A function implemented by the game, this function is called when the game engine updates.
 * \param[in] elglr The ELGLR instance used.
 * \param[in] currentTime Use this time for animations.
 * \param[in] fps The current fps.
 * \return An error code.
 */
rsge_error_e rsge_game_update(rsge_elglr_t* elglr,double currentTime,int fps);

/**
 * \fn rsge_error_e rsge_game_uninit(rsge_elglr_t* elglr)
 * \brief A function implemented by the game, this function is called when the game engine uninitializes or quits.
 * \param[in] elglr The ELGLR instance used.
 * \return An error code.
 */
rsge_error_e rsge_game_uninit(rsge_elglr_t* elglr);

#endif
