
#include "ECS.cpp"

#include <vector>
#include <typeindex>

class Evento : public GameObject {
public:
	Evento() {};

};
// Manera 1 para generar eventos : composicion -> El evento contiene su tipo
// Manera 2 : herencia -> subtipos de evento -> más cómodo

class Error : public Evento {
public:
	Error(uint32_t nivel, std::string mensaje) {
		this->nivel = nivel;
		this->mensaje = mensaje;
	}

	enum NivelError {
		DEBUG = 0,
		INFO,
		WARN,
		FATAL
	};

protected:
	uint32_t nivel;
	std::string mensaje;
};



// Sistema de eventos es un Sistema
// Que usa callbacks -> funciones que van a handlers (manejadores)

// El sistema de eventos no ejecuta los eventos.
// Al mover el ratón, se mueve la cámara -> el evento "mover ratón" lo causa el sistema de inputs
// la cámara la mueve la propia cámara
// hace de conector entre ellos

typedef void (*callbackEvento)(GameObject* obj, Evento* evento);
// debe ser void
// debe tener parametros GameObject* y Evento*
// debe ser funcion y no método

class SistemaEventos : public Sistema {
public:
	void inicializar() override {}
	void actualizar(float delta) override {}
	void apagar() override {}

	class Suscriptor { // Sistemas que manejan eventos, por ejemplo el SistemaCamara
	public:
		Suscriptor() : tipo(typeid(this)), suscriptor(0), callback(0) {}

		std::type_index tipo;
		GameObject* suscriptor; // Este es el objeto que va a manejar el evento
		callbackEvento callback; // Esto es lo que debe hacer el objeto que maneja el evento
	};

	// definiendo la respuesta. El obj reaccionará con callback a el evento T
	template<class TipoEvento>
	void suscribir(GameObject* obj, callbackEvento callback) {
		Suscriptor* nuevo_suscr = new Suscriptor(); // Almacenamiento de objetos que van a reaccionar a eventos
		nuevo_suscr->tipo = typeid(TipoEvento);
		nuevo_suscr->suscriptor = obj;
		nuevo_suscr->callback = callback;

		suscriptores.push_back(nuevo_suscr); // añadir a la lista
	}

	template<class TipoEvento>
	void publicar(TipoEvento* evento) { // Ha ocurrido un evento TipoEvent
		std::type_index tipo = typeid(TipoEvento);

		for (auto it = suscriptores.begin(); it < suscriptores.end(); it++) { // para todos los objetos guardados
			if (tipo == (*it)->tipo) { // Si tengo un suscriptor que va a reaccionar a ese tipo de evento
				if ((*it)->suscriptor != 0) { // y no es nulo
					(*it)->callback(   (*it)->suscriptor , evento   ); // Haz el callback que guardé
				}

			}
		}
	}

protected:
	std::vector<Suscriptor*> suscriptores;
};

