#include <iostream>
#include <cstring>
#include <mosquitto.h>

int id_buscada = 12345; // ID predeterminada para simular la búsqueda en la base de datos

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg);

bool buscar_en_bd(int id) {
    // Función que busca el ID en la base de datos y devuelve true o false según si existe o no.
    // En este caso, simplemente comparamos el ID buscado con el ID predeterminado.
    return (id == id_buscada);
}

int main(int argc, char *argv[]) {
    struct mosquitto *mosq = NULL;
    mosquitto_lib_init();

    // Crear una instancia de cliente Mosquitto
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        std::cerr << "Error: No se pudo crear la instancia del cliente Mosquitto.\n";
        mosquitto_lib_cleanup();
        return 1;
    }

    // Conectar el cliente Mosquitto al broker
    if (mosquitto_connect(mosq, "localhost", 1883, 60) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Error: No se pudo conectar al broker Mosquitto.\n";
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }

    // Suscribirse a un tema MQTT
    if (mosquitto_subscribe(mosq, NULL, "autenticacion", 0) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Error: No se pudo suscribir al tema MQTT.\n";
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }

    // Procesar los mensajes MQTT recibidos
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_loop_forever(mosq, -1, 1);

    // Desconectar el cliente Mosquitto
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    // Obtener la ID desde el mensaje recibido
    int id = atoi((char *)msg->payload);

    // Realizar la búsqueda en la base de datos y obtener el resultado
    bool existe_en_bd = buscar_en_bd(id);

    // Responder al broker con el resultado de la búsqueda
    if (existe_en_bd) {
        mosquitto_publish(mosq, NULL, "respuesta", strlen("existe"), "existe", 0, false);
    } else {
        mosquitto_publish(mosq, NULL, "respuesta", strlen("no existe"), "no existe", 0, false);
    }
}
