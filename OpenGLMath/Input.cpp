
#pragma once

#include "ECS.hpp"
#include "Event.cpp"

#include <map>

class EventoInput : public Evento {
public:
	EventoInput(std::string accion) {
		this->accion;
		this->cantidad = 0;
	}
	~EventoInput() = default;

	std::string accion;
	float cantidad; // botones analogicos
};



class SistemaInput : public Sistema {
protected:

	struct Mapeo { // son botones, pero guardan tambien la accion para los callback y los eventos
		std::string accion;
		int boton;
		// aparecería el dispositivo
	};

	
	std::map<std::string, std::vector<Mapeo*>> acciones;
	// Mencion - se podria tener clases para raton, teclado y mando. En ese caso, habria que hacer otro mapa con relaciones entre esos dispositivos y los botones

public:
	SistemaInput() {
		acciones = std::map<std::string, std::vector<Mapeo*>>();
		acciones.empty();
	}
	virtual ~SistemaInput() = default;

	void inicializar() override {
		// comprobar dispositivos
	}

	void actualizar(float delta) override {
		// comprobar dispositivos y si se desconectan/actualizan
	}

	void apagar() override {

	}

	void registrarMapeos(int boton, std::string accion) {
		// Comprobar si la accion ya existe - ya tiene mapeo
		auto it = acciones.find(accion);
		if (it == acciones.end()) { // no existe
			acciones[accion] = std::vector<Mapeo*>();
		}

		// Lo mismo para dispositivos

		// Crear el mapeo
		Mapeo* mapeo = new Mapeo();
		mapeo->accion = accion;
		mapeo->boton = boton;

		acciones[accion].push_back(mapeo);
	}

	void lanzarAccion(int boton) {
		SistemaEventos* sistema_eventos = GameLoop::getInstance()->buscarSistema<SistemaEventos>();

		for (auto it_map = acciones.begin(); it_map != acciones.end(); it_map++) {
			for (auto it_vec = it_map->second.begin(); it_vec != it_map->second.end(); it_vec++) {
				if ((*it_vec)->boton == boton) {
					// Esto está aquí suponiendo que la accion tiene un callback asociado
					sistema_eventos->publicar(new EventoInput((*it_vec)->accion) ); 
				}
			}
		}
	}
};
