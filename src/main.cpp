#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <GeometryDash/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    // Interceptamos la actualización del nivel por cada frame
    void update(float dt) {
        // Ejecuta primero el comportamiento original del juego
        PlayLayer::update(dt);

        // Aseguramos que los objetos del jugador existan en la partida
        if (!m_player1 || !m_player2) return;

        // 1. Obtener el gestor del mapa de efectos de color del nivel
        auto colorActionSpriteMgr = m_levelEffectLayer;
        if (!colorActionSpriteMgr) return;

        // ID 1000 = Color del Background (BG)
        // ID 1001 = Color del Ground 1 (G1)
        cocos2d::ccColor3B bg_color = colorActionSpriteMgr->getColorValue(1000);
        cocos2d::ccColor3B g_color = colorActionSpriteMgr->getColorValue(1001);

        // 2. Aplicar BG al Color 1 del Cubo 1 y del Cubo 2 (Modo Dual)
        m_player1->setColor(bg_color);
        m_player2->setColor(bg_color);

        // 3. Aplicar G al Color 2 del Cubo 1 y del Cubo 2
        m_player1->setSecondColor(g_color);
        m_player2->setSecondColor(g_color);
    }
};
