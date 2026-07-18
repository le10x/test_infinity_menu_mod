#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <string>
#include <sstream>
#include <iomanip>

using namespace geode::prelude;

// Tu función utilitaria de formateo
std::string formatDownloads(int count) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << (count / 1000.0) << "k";
    return ss.str();
}

// El Hook compatible con Geode 5.8.2 / GD 2.2081
class $modify(MyLevelCell, LevelCell) {
    void loadCustomLevelCell() {
        LevelCell::loadCustomLevelCell();
        
        // Comprobación de seguridad para evitar crashes en celdas vacías
        if (!m_level) return;

        int downloads = m_level->m_downloads;
        std::string formattedText = formatDownloads(downloads);
        
        // Buscar el nodo usando la nomenclatura oficial de Geode v5
        if (auto downloadLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("downloads-label"))) {
            downloadLabel->setString(formattedText.c_str());
        }
    }
};
