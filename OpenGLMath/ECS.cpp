

#include "ECS.hpp"


// Crea componentes o sus subtipos
template<class T> Componente* Entidad::crearComponente() {
	Componente* nuevo_componente = dynamic_cast<Componente*> (new T());
	if (nuevo_componente) {
		componentes.push_back(nuevo_componente);
	}
	return nuevo_componente;
}

// Busca si en los componentes hay un objeto de tipo T
template<class T> T* Entidad::buscarComponente() {
	for (auto it = componentes.begin(); it < componentes.end(); it++) {
		T* obj = dynamic_cast<T*>(*it);
		if (obj) return obj;
	}
	return 0;
}

// Añadir componentes existentes
void Entidad::addComponente(Componente* nuevo_componente) {
	componentes.push_back(nuevo_componente);
}

// Eliminar componentes
void Entidad::eliminarComponentes(Componente* componente_a_borrar) {
	auto it = std::find(componentes.begin(), componentes.end(), componente_a_borrar);
	if (it != componentes.end()) {
		componentes.erase(it);
	}
}




// Crea entidades o sus subtipos
Entidad* Mundo::crearEntidad(uint32_t id) {
	Entidad* nueva_entidad;

	switch (id) {
	case 0:
		//nueva_entidad = new Player(id);
		break;
	}		

	//Entidad* nuevo_componente = new Entidad();
	entidades.insert({ id, nueva_entidad });
	return nueva_entidad;
}

// Añadir entidades existentes
void Mundo::addEntidad(Entidad* nueva_entidad) {
	entidades.insert({ nueva_entidad->id, nueva_entidad});
}


// Busca si en los componentes hay un objeto de tipo T
template<class T> std::vector<T*> Mundo::buscarComponentes() {

	std::vector<T*> componentes;
	for (auto it : entidades) {
		T* posible_componente = it.second->buscarComponente<T>();
		if (posible_componente) componentes.push_back(posible_componente);
	}
	return componentes;
}


// Eliminar componentes
void Mundo::eliminarEntidad(Entidad* entidad_a_borrar) {
	//auto it = std::find(entidades.begin(), entidades.end(), entidad_a_borrar);
	//if (it != entidades.end()) {
	//	entidades.erase(it);
	//}
}

Mundo* Mundo::instancia = 0;
Mundo* Mundo::getInstance() {
	if (!instancia) {
		instancia = new Mundo();
	}
	return instancia;
}




void GameLoop::Tick() {
	tick = glfwGetTime();
	float delta = tick - ultimo_tick;
	ultimo_tick = tick;

	for (auto it = sistemas.begin(); it < sistemas.end(); it++) {
		SistemaRegistrado* sr = dynamic_cast<SistemaRegistrado*>(*it);
		if (delta > sr->tick_rate) {
			sr->sistema->actualizar(delta);
			sr->ticker = 0;
		}
		else {
			sr->ticker += delta;
		}
	}
}

// Crea sistemas o sus subtipos
template<class T> T* GameLoop::crearSistema(float tick_rate) {
	T* nuevo_sistema = new T();
	nuevo_sistema->inicializar();

	SistemaRegistrado* sr = new SistemaRegistrado();
	sr->sistema = nuevo_sistema;
	sr->tick_rate = (tick_rate == 0) ? 0 : 1.0f / tick_rate;
	sr->ticker = 0;

	return nuevo_sistema;
}

// Añadir sistemas
void GameLoop::addSistema(Sistema* sistema, float tick_rate) {
	sistema->inicializar();

	SistemaRegistrado* sr = new SistemaRegistrado();
	sr->sistema = sistema;
	sr->tick_rate = (tick_rate == 0) ? 0 : 1.0f / tick_rate;
	sr->ticker = 0;
	std::cout << sr;

	sistemas.push_back(sr);
}

GameLoop* GameLoop::instancia = 0;
GameLoop* GameLoop::getInstance() {
	if (!instancia) {
		instancia = new GameLoop();
	}
	return instancia;
}


