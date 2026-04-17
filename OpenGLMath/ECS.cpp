
#pragma

#include <iostream>
#include <vector>
#include <map>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm.hpp>


class GameObject {
public:
	GameObject() { tipo.id_tipo = 0; };
	// destructor

	struct Tipo {
		std::string nombre_tipo;
		uint32_t id_tipo;
	};
protected:
	Tipo tipo;
};


class Componente;
class Entidad;
class Sistema;


// IMPORTANTE : clases genéricas (no son abstractas, 
// pero si que pretenden ser lo más genéricas posible 
// para que pueda heredar la mayor cantidad de clases sin limitarme)

class Componente : public GameObject {
	// Objetivo: contener datos o propiedades
public:
	Componente() : entidad(0) {};

	Entidad* entidad; // Lo unico que puede decir de un componente es que está dentro de una entidad
};



class Entidad : public GameObject {
	// Objetivo: contener una serie de componentes y poder localizarlos
protected:
	std::vector<Componente*> componentes;
	
public:
	uint32_t id;
	Entidad(uint32_t id) : id(id) {}

	// Crea componentes o sus subtipos
	template<class T> T* crearComponente() {
		T* nuevo_componente = new T();
		componentes.push_back(nuevo_componente);
		return nuevo_componente;
	}

	// Busca si en los componentes hay un objeto de tipo T
	template<class T> T* buscarComponente() {
		for (auto it = componentes.begin(); it < componentes.end(); it++) {
			T* obj = dynamic_cast<T*>(*it);
			if (obj) return obj;
		}
		return 0;
	}

	// Añadir componentes existentes
	void addComponente(Componente* nuevo_componente) {
		componentes.push_back(nuevo_componente);
	}

	// Eliminar componentes
	void eliminarComponentes(Componente* componente_a_borrar) {
		auto it = std::find(componentes.begin(), componentes.end(), componente_a_borrar);
		if (it != componentes.end()) {
			componentes.erase(it);
		}
	}
};



class Sistema : public GameObject {
	// Procesar comportamientos
public:
	virtual void inicializar() {}
	virtual void actualizar(float delta) {}
	virtual void apagar() {}
};

class SistemaParticulas : public Sistema {
	void draw() {}
};



class Mundo : public GameObject {
protected:
	std::map<uint32_t, Entidad*> entidades;
	uint32_t siguiente_id_vacia;
	static Mundo* instancia;
	Mundo() {}

public:
	// Singleton
	static Mundo* getInstance();
	Mundo(Mundo& other) = delete;
	void operator=(const Mundo&) = delete;

	// Crea entidades o sus subtipos
	Entidad* crearEntidad(uint32_t id = 0) {
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
	void addEntidad(Entidad* nueva_entidad) {
		entidades.insert({ nueva_entidad->id, nueva_entidad});
	}


	// Busca si en los componentes hay un objeto de tipo T
	template<class T> std::vector<T*> buscarComponentes() {

		std::vector<T*> componentes;
		for (auto it : entidades) {
			T* posible_componente = it.second->buscarComponente<T>();
			if (posible_componente) componentes.push_back(posible_componente);
		}
		return componentes;
	}


	// Eliminar componentes
	void eliminarEntidad(Entidad* entidad_a_borrar) {
		//auto it = std::find(entidades.begin(), entidades.end(), entidad_a_borrar);
		//if (it != entidades.end()) {
		//	entidades.erase(it);
		//}
	}

	Entidad* buscarEntidad(uint32_t id); // comodidad

};

Mundo* Mundo::instancia = 0;
Mundo* Mundo::getInstance() {
	if (!instancia) {
		instancia = new Mundo();
	}
	return instancia;
}



class GameLoop : public GameObject {

protected:
	float tick, ultimo_tick;
	static GameLoop* instancia;
	GameLoop() {}

	struct SistemaRegistrado {
		Sistema* sistema;
		float tick_rate; // cada cuanto me actualizo
		float ticker; // por cual voy
	};

	std::vector<SistemaRegistrado*> sistemas;

public:
	// Singleton
	static GameLoop* getInstance();
	GameLoop(GameLoop& other) = delete;
	void operator=(const GameLoop&) = delete;

	void Tick() {
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
	template<class T> T* crearSistema(float tick_rate = 0) {
		T* nuevo_sistema = new T();
		nuevo_sistema->inicializar();

		SistemaRegistrado* sr = new SistemaRegistrado();
		sr->sistema = nuevo_sistema;
		sr->tick_rate = (tick_rate == 0) ? 0 : 1.0f / tick_rate;
		sr->ticker = 0;

		sistemas.push_back(sr);
		return nuevo_sistema;
	}

	// Encontrar sistema
	template<class T> T* buscarSistema() {
		for (auto it = sistemas.begin(); it < sistemas.end(); it++) {
			T* obj = dynamic_cast<T*>(*it);
			if (obj) return obj;
		}
		return 0;
	}

};

GameLoop* GameLoop::instancia = 0;
GameLoop* GameLoop::getInstance() {
	if (!instancia) {
		instancia = new GameLoop();
	}
	return instancia;
}

