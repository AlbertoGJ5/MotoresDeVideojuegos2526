
#pragma

#include <iostream>
#include <vector>
#include <map>

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
	virtual void actualizarse(float delta) {}
	virtual void apagar() {}
};

class SistemaParticulas : public Sistema {
	void draw() {}
};





