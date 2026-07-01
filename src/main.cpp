#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

bool g_isRouletteActive = false;       
bool g_isNoclipEnabled = false;        
bool g_hasPendingNotification = false; 

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        std::string side = Mod::get()->getSettingValue<std::string>("button-side");
        std::string menuID = (side == "Left") ? "left-button-menu" : "right-button-menu";
        auto menu = this->getChildByID(menuID);
        if (!menu) return;

        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("?", "goldFont.fnt", "GJ_button_02.png", 0.7f),
            this, menu_selector(MyPauseLayer::Press)
        );
        menu->addChild(btn);
        menu->updateLayout();
    }

    void Press(CCObject*) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        
        // Regresa al 50/50 original
        g_isNoclipEnabled = (std::rand() % 2 == 1);
        
        g_isRouletteActive = true;
        g_hasPendingNotification = true;
        
        FLAlertLayer::create("Random Noclip", "Noclip enabled? Find out by playing!", "Ok") -> show();
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    void triggerNoclipNotification() {
        if (g_hasPendingNotification) {
            g_hasPendingNotification = false; 
            if (g_isNoclipEnabled) {
                Notification::create("Noclip was activated!", NotificationIcon::Success, 2.0f) -> show();
            } else {
                Notification::create("Noclip was disabled!", NotificationIcon::Error, 2.0f) -> show();
            }
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        g_isNoclipEnabled = false;
        g_isRouletteActive = false;
        g_hasPendingNotification = false;
    }

    void onQuit() {
        PlayLayer::onQuit();
        g_isNoclipEnabled = false;
        g_isRouletteActive = false;
        g_hasPendingNotification = false;
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (g_isRouletteActive) {
            triggerNoclipNotification();
        }

        if (g_isNoclipEnabled) {
            return;
        }

        PlayLayer::destroyPlayer(player, object);
    }

    void levelComplete() {
        if (g_isRouletteActive && g_hasPendingNotification) {
            triggerNoclipNotification();
        }

        g_isNoclipEnabled = false;
        g_isRouletteActive = false;
        g_hasPendingNotification = false;

        PlayLayer::levelComplete();
    }
};
