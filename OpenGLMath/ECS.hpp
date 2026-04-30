
#pragma once

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
	virtual ~GameObject() = default; // polimorfico - puede tener destruccion como otro objeto

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
	template<class T> Componente* crearComponente();

	// Busca si en los componentes hay un objeto de tipo T
	template<class T> T* buscarComponente();

	// Añadir componentes existentes
	void addComponente(Componente* nuevo_componente);

	// Eliminar componentes
	void eliminarComponentes(Componente* componente_a_borrar);
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
	Entidad* crearEntidad(uint32_t id = 0);

	// Añadir entidades existentes
	void addEntidad(Entidad* nueva_entidad);

	// Busca si en los componentes hay un objeto de tipo T
	template<class T> std::vector<T*> buscarComponentes();

	// Eliminar componentes
	void eliminarEntidad(Entidad* entidad_a_borrar);
	Entidad* buscarEntidad(uint32_t id); // comodidad
};




class GameLoop : public GameObject {

protected:
	float tick, ultimo_tick;
	static GameLoop* instancia;
	GameLoop() {}

	struct SistemaRegistrado {
		virtual ~SistemaRegistrado() = default;

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

	void Tick();

	// Crea sistemas o sus subtipos
	template<class T> T* crearSistema(float tick_rate = 0);

	void addSistema(Sistema* sistema, float tick_rate = 0);

	// Encontrar sistema
	template<class T>
	T* buscarSistema() {
		for (auto it = sistemas.begin(); it < sistemas.end(); it++) {
			T* obj = dynamic_cast<T*>(*it);
			if (obj) return obj;
		}
		return 0;
	}

};