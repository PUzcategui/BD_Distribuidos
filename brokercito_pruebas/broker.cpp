#include <stdio.h>
#include <mosquitto.h>

void on_connect(struct mosquitto *mosq, void *userdata, int rc) {
    if (rc == 0) {
        printf("Broker conectado\n");
    }
    else {
        printf("Error al conectar con el broker: %s\n", mosquitto_connack_string(rc));
    }
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    printf("Mensaje recibido en el topic %s: %.*s\n", msg->topic, msg->payloadlen, (char *)msg->payload);
    // Aquí es donde procesamos el mensaje recibido y hacemos la autenticación correspondiente
}

int main(int argc, char *argv[]) {
    struct mosquitto *mosq;
    int rc;

    // Inicializamos Mosquitto
    mosquitto_lib_init();

    // Creamos un nuevo mosquitto
    mosq = mosquitto_new("broker", true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error: No se pudo crear el mosquitto\n");
        return 1;
    }

    // Configuramos los callbacks
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    // Conectamos con el broker
    rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar con el broker: %s\n", mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        return 1;
    }

    // Nos subscribimos al topic "autenticacion"
    rc = mosquitto_subscribe(mosq, NULL, "autenticacion", 0);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al suscribirse al topic: %s\n", mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        return 1;
    }

    // Bucle principal de Mosquitto
    rc = mosquitto_loop_forever(mosq, -1, 1);

    // Limpiamos y cerramos Mosquitto
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
