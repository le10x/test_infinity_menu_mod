#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {
    void pushButton(PlayerButton button) {
        // Ejecuta primero la lógica normal del juego
        PlayerObject::pushButton(button);

        // Cambia la gravedad solo con el botón de salto
        if (button == PlayerButton::Jump) {
            // Se añaden los argumentos requeridos: (true = voltear, false = con efectos)
            this->flipGravity(true, false);
        }
    }
};
