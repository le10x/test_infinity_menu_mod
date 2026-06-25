#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

bool g_suerteDecidida = false;
bool g_noclipSecreto = false;

// 1. CREACIÓN DEL BOTÓN DE SUERTE EN EL MENÚ DE PAUSA
class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        auto menu = this->getChildByID("right-button-menu");
        if (!menu) return;

        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("¿?", "goldFont.fnt", "GJ_button_02.png", 0.7f),
            this, menu_selector(MyPauseLayer::alPresionar)
        );
        menu->addChild(btn);
        menu->updateLayout();
    }

    void alPresionar(CCObject*) {
        std::srand(static_cast<unsigned int>(std::time(nullptr))); // Inicializa el azar real
        g_noclipSecreto = (std::rand() % 2 == 1);
        g_suerteDecidida = true;
        FLAlertLayer::create("Destino Sellado", "El azar decidió tu suerte en secreto. <y>Averígualo jugando...</y>", "¡Ok!") -> show();
    }
};

// 2. INTERCEPCIÓN DIRECTA DE LA MUERTE DEL ICONO
class $modify(MyPlayerObject, PlayerObject) {
    void playerDestroyed(bool p0) {
        if (g_suerteDecidida) {
            g_suerteDecidida = false; // Detiene alertas repetidas
            if (g_noclipSecreto) {
                Notification::create("¡BENDITO POR EL AZAR! (Noclip Activo)", NotificationIcon::Success, 1.5f) -> show();
            } else {
                Notification::create("¡EL DESTINO TE ABANDONÓ!", NotificationIcon::Error, 2.0f) -> show();
            }
        }

        // Si la ruleta dio Noclip Secreto, bloqueamos la destrucción regresando de inmediato
        if (g_noclipSecreto) return;

        // Si no se activó la suerte, el icono explota de forma normal
        PlayerObject::playerDestroyed(p0);
    }
};
