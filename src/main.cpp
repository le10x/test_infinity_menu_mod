#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        // Buscar el menú lateral izquierdo de la pantalla de información del nivel
        if (auto menu = this->getChildByID("left-side-menu")) {
            
            // Crear un botón circular verde con la textura nativa del pulgar hacia arriba de GD
            auto myButton = CCMenuItemSpriteExtra::create(
                CircleButtonSprite::createWithSpriteFrameName("GJ_likeBtn_001.png", 1.0f, CircleBaseColor::Green),
                this,
                menu_selector(MyLevelInfoLayer::onAlternativeLike)
            );

            // Asignarle una ID al botón para mantener una buena estructura en Geode
            myButton->setID("alt-like-button");

            // Añadir el botón al menú lateral y reorganizar los elementos de forma automática
            menu->addChild(myButton);
            menu->updateLayout();
        }

        return true;
    }

    void onAlternativeLike(CCObject* sender) {
        // Validación de seguridad para que Android no crashee si los punteros están vacíos
        if (m_level && GameLevelManager::sharedState()) {
            
            // Enviamos el LIKE directo saltándonos el bloqueo de la partida guardada local
            // Argumentos: (Tipo de Item, ID del nivel, esLike?, parentID)
            GameLevelManager::sharedState()->likeItem(
                static_cast<LikeItemType>(1), 
                m_level->m_levelID, 
                true, 
                0
            );

            // Alerta visual en pantalla que confirma la ejecución del truco
            FLAlertLayer::create("Geode", "¡Like enviado!", "OK")->show();
        }
    }
};
