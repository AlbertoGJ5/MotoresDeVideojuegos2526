
#pragma once

#include "ECS.cpp"

Mundo* Mundo::instancia = 0;
Mundo* Mundo::getInstance() {
	if (!instancia) {
		instancia = new Mundo();
	}
	return instancia;
}


GameLoop* GameLoop::instancia = 0;
GameLoop* GameLoop::getInstance() {
	if (!instancia) {
		instancia = new GameLoop();
	}
	return instancia;
}