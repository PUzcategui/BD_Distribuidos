#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mosquitto.h>

#define ADDRESS     "localhost"
#define PORT        1883
#define CLIENTID    "Pantalla"
#define TOPIC       "autenticacion"
#define QOS         1
#define TIMEOUT     -1

volatile int finished = 0;

void handle_signal(int s) {
    finished = 1;
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    printf("Mensaje recibido en el topic %s: %.*s\n", message->topic, message->payloadlen, (char *)message->payload);
    // Aquí es donde procesamos la respuesta de la API
    mosquitto_disconnect(mosq);
}

int main(int argc, char* argv[]) {
    struct mosquitto *mosq;
    int rc;
    char payload[100];

    signal(SIGINT, handle_signal);

    // Inicializamos Mosquitto
    mosquitto_lib_init();

    // Creamos un nuevo mosquitto
    mosq = mosquitto_new(CLIENTID, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error: No se pudo crear el mosquitto\n");
        return 1;
    }

    // Nos conectamos al broker MQTT
    rc = mosquitto_connect(mosq, ADDRESS, PORT, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectarse al broker MQTT: %s\n", mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        return 1;
    }

    // Configuramos los callbacks
    mosquitto_message_callback_set(mosq, on_message);

    while (!finished) {
        // Leemos el número de ID a verificar desde la pantalla
        printf("Ingrese el número de ID a verificar: ");
        fgets(payload, 100, stdin);
        // Quitamos el '\n' del final de la cadena
        payload[strcspn(payload, "\n")] = '\0';

        // Publicamos el número de ID en el topic "autenticacion"
        rc = mosquitto_publish(mosq, NULL, TOPIC, strlen(payload), payload, QOS, false);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Error al publicar mensaje: %s\n", mosquitto_strerror(rc));
            mosquitto_destroy(mosq);
            return 1;
        }

        // Esperamos la respuesta de la API
        while (mosquitto_loop(mosq, TIMEOUT, 1) == MOSQ_ERR_SUCCESS && !finished) {
            // Esperamos a que llegue el mensaje
        }
    }

    // Limpiamos y cerramos Mosquitto
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
