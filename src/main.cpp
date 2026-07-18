#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        // 1. Primero obligamos al nivel a "olvidar" que ya le diste Like antes de cargar la interfaz
        if (level) {
            level->m_likeStatus = 0; // Ponemos el estado de voto local en 0 (sin votar)
        }

        // 2. Cargamos la pantalla normalmente
        if (!LevelInfoLayer::init(level, p1)) return false;

        // 3. Buscamos el botón original de Like para asegurarnos de que permanezca activo
        if (auto menu = this->getChildByID("like-menu")) {
            if (auto likeBtn = menu->getChildByID("like-button")) {
                // Forzamos al botón original a estar siempre disponible/activado
                if (auto btnItem = typeinfo_cast<CCMenuItemSpriteExtra*>(likeBtn)) {
                    btnItem->setEnabled(true);
                    btnItem->setColor({255, 255, 255}); // Le quitamos el color grisáceo si se lo puso
                }
            }
        }

        return true;
    }
};
