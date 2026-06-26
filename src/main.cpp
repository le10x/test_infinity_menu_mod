#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

// Variables de estado global
bool g_yesclip = false;
bool g_noclip = false;

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        // Leer el ajuste de posición desde el mod.json
        std::string side = Mod::get()->getSettingValue<std::string>("button-side");
        
        // Identificar el menú correspondiente
        std::string menuID = (side == "Left") ? "left-button-menu" : "right-button-menu";
        auto menu = this->getChildByID(menuID);
        
        if (!menu) return;

        // Crear el botón de la ruleta rusa
        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("?", "goldFont.fnt", "GJ_button_02.png", 0.7f),
            this, menu_selector(MyPauseLayer::Press)
        );
        menu->addChild(btn);
        menu->updateLayout();
    }

    void Press(CCObject*) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        g_noclip = (std::rand() % 2 == 1);
        g_yesclip = true; // Activa el verificador para el choque inminente
        FLAlertLayer::create("Random Noclip", "Noclip enabled? Find out by playing!", "Ok") -> show();
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    // CORRECCIÓN 2: Limpiar estados al reiniciar el nivel
    void resetLevel() {
        PlayLayer::resetLevel();
        g_noclip = false;
        g_yesclip = false;
    }

    // CORRECCIÓN 2: Limpiar estados al salir del nivel al menú principal
    void onQuit() {
        PlayLayer::onQuit();
        g_noclip = false;
        g_yesclip = false;
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        // CORRECCIÓN 1: La notificación se ejecuta EXCLUSIVAMENTE al registrarse una colisión real
        if (g_yesclip) {
            g_yesclip = false; // Desactivamos el validador para que solo avise una vez
            if (g_noclip) {
                Notification::create("Noclip was activated!", NotificationIcon::Success, 2.0f) -> show();
            } else {
                Notification::create("Noclip was disabled!", NotificationIcon::Error, 2.0f) -> show();
            }
        }

        // Si la ruleta asignó inmunidad, eludimos el código destructivo del juego
        if (g_noclip) {
            return;
        }

        // Si tocó mala suerte, el jugador muere de forma convencional
        PlayLayer::destroyPlayer(player, object);
    }

    void levelComplete() {
        // CORRECCIÓN 2: Si el jugador gana limpiamente, reseteamos el estado sin lanzar alertas molestas
        g_noclip = false;
        g_yesclip = false;
        PlayLayer::levelComplete();
    }
};
