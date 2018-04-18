#pragma once

#include "..\stdbase.hpp"

struct Mouse {

	SINCE(0, 0, 3)
	virtual void mouse_move(glm::vec2 pos, glm::vec2 deltapos) {};

};

extern glm::vec2 last_pos;
extern std::vector<Mouse*> mouse_handler;

SINCE(0, 0, 3)
void inputupdate(glm::vec2 pos);

SINCE(0, 0, 3)
void addListener(Mouse* mouse);