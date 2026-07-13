#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    void update(float dt) {
        PlayLayer::update(dt);

        if (!m_player1 || !m_player2) return;

        // Accedemos al gestor de efectos del juego
        auto gameMgr = GameManager::sharedState();
        if (!gameMgr) return;

        // ID 1000 = Color del Background (BG)
        // ID 1001 = Color del Ground 1 (G1)
        cocos2d::ccColor3B bg_color = gameMgr->colorForEffectID(1000);
        cocos2d::ccColor3B g_color = gameMgr->colorForEffectID(1001);

        // Aplicar BG al Color 1 de ambos cubos
        m_player1->setColor(bg_color);
        m_player2->setColor(bg_color);

        // Aplicar G al Color 2 de ambos cubos
        m_player1->setSecondColor(g_color);
        m_player2->setSecondColor(g_color);
    }
};
