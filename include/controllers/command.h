#ifndef COMMAND_H
#define COMMAND_H

#include <optional>
#include <string>
#include "field/position.h"

enum class CommandType
{
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Attack,
    Quit,
    OpenHand,
    ChangeWeapon,
    Save,
    SelectPosition,
    UseSpell,
    SelectIndex,
    SelectSpellFromHand,
    SelectFilename,
    Load,
    LevelUp,
    Confirm,
    None
};

struct Command
{
    CommandType type;

    std::optional<Position> position;
    std::optional<int> index;
    std::optional<std::string> text;
    std::optional<char> opt;
};

#endif