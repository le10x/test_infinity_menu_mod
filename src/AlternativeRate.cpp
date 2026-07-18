#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(RateLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        if (auto menu = this->getChildByID("left-side-menu")) {
            auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
            
            if (btnSprite) {
                auto myButton = CCMenuItemSpriteExtra::create(
                    btnSprite,
                    this,
                    menu_selector(RateLayer::onAlternativeRateStars)
                );

                myButton->setID("alt-rate-stars-button");
                menu->addChild(myButton);
                menu->updateLayout();
            }
        }
        return true;
    }

    void onAlternativeRateStars(CCObject* sender) {
        if (m_level && GameLevelManager::sharedState()) {
            
            // Leemos el valor seleccionado por el usuario en los ajustes del mod
            int64_t estrellasElegidas = Mod::get()->getSettingValue<int64_t>("cantidad-estrellas");

            // Enviamos la petición de red con las estrellas dinámicas
            GameLevelManager::sharedState()->rateStars(m_level->m_levelID, static_cast<int>(estrellasElegidas));

            // Alerta que muestra dinámicamente cuántas estrellas enviaste
            std::string mensaje = "Voto de " + std::to_string(estrellasElegidas) + " estrellas enviado.";
            FLAlertLayer::create("Geode", mensaje.c_str(), "OK")->show();
        }
    }
};
