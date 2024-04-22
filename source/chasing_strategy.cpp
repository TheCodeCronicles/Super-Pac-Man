#include "game.h"

// Strategy 1: Basic chasing strategy
QPair<int, int> strategy1(Ghost* ghost)
{
    // Get a pointer to the pacman object from the game
    Pacman *pacman = ghost->game->pacman;

    // Calculate the distance between the ghost and pacman along the x and y axes
    int dist_x = pacman->get_x() - ghost->get_x();
    int dist_y = pacman->get_y() - ghost->get_y();

    // Return the pair of distances
    return QPair<int, int>(dist_x, dist_y);
}

// Strategy 2: Chasing strategy with a slight offset based on pacman's direction
QPair<int, int> strategy2(Ghost* ghost)
{
    // Get pointers to the pacman object and the game's red ghost object
    Pacman *pacman = ghost->game->pacman;
    Ghost *red_ghost = ghost->game->ghost[Ghost::Red];

    // Calculate the distance between the ghost and pacman along the x and y axes
    int dist_x = pacman->get_x() - ghost->get_x();
    int dist_y = pacman->get_y() - ghost->get_y();

    // Adjust the distances based on pacman's current direction
    switch(pacman->get_dir())
    {
    case GameObject::Up:
        dist_y -= 4;
        break;
    case GameObject::Down:
        dist_y += 4;
        break;
    case GameObject::Left:
        dist_x -= 4;
        break;
    case GameObject::Right:
        dist_x += 4;
        break;
    default:
        break;
    }

    // Return the pair of distances
    return QPair<int, int>(dist_x, dist_y);
}

// Strategy 3: Chasing strategy to flank pacman by aiming for a point mirrored from the red ghost
QPair<int, int> strategy3(Ghost* ghost)
{
    // Get pointers to the pacman object, the game's red ghost object, and the ghost itself
    Pacman *pacman = ghost->game->pacman;
    Ghost *red_ghost = ghost->game->ghost[Ghost::Red];

    // Calculate the distance between the ghost and pacman along the x and y axes,
    // aiming for a point mirrored from the red ghost relative to pacman's position
    int dist_x = (pacman->get_x() - red_ghost->get_x()) * 2 - ghost->get_x();
    int dist_y = (pacman->get_y() - red_ghost->get_y()) * 2 - ghost->get_y();

    // Return the pair of distances
    return QPair<int, int>(dist_x, dist_y);
}

// Strategy 4: Chasing strategy to retreat if close to pacman, otherwise follow pacman
QPair<int, int> strategy4(Ghost* ghost)
{
    // Get a pointer to the pacman object from the game
    Pacman *pacman = ghost->game->pacman;

    // Calculate the distance between the ghost and pacman along the x and y axes
    int dist_x = pacman->get_x() - ghost->get_x();
    int dist_y = pacman->get_y() - ghost->get_y();

    // If the ghost is close to pacman, retreat towards a fixed point, else continue chasing pacman
    if (dist_x * dist_x + dist_y * dist_y < 64) { // 64 is the squared distance threshold (8^2)
        dist_x = 2 - ghost->get_x(); // Retreat towards a fixed x-coordinate
        dist_y = 2 - ghost->get_y(); // Retreat towards a fixed y-coordinate
    }

    // Return the pair of distances
    return QPair<int, int>(dist_x, dist_y);
}
