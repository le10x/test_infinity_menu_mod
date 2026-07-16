#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>

using namespace geode::prelude;

// Interceptamos la clase que controla cada celda de comentario individual
class $modify(MyCommentCell, CommentCell) {
    
    // Este método se ejecuta al cargar y dibujar el comentario en pantalla
    void loadFromCell(GJComment* comment, int cellType) {
        // 1. Dejamos que el juego ejecute su lógica original primero
        // Aquí es donde el GD vainilla bloquea los botones ya votados o propios
        CommentCell::loadFromCell(comment, cellType);

        // 2. Rompemos las restricciones del botón de Like
        if (this->m_likeBtn) {
            // Forzamos el botón a estar SIEMPRE activo (true)
            // Esto reactiva los ya votados y tus comentarios propios al instante
            this->m_likeBtn->setEnabled(true);
            
            // Nos aseguramos de que el botón mantenga su opacidad al 100%
            // El juego original los vuelve semitransparentes al bloquearlos
            this->m_likeBtn->setOpacity(255);
            this->m_likeBtn->setColor({255, 255, 255});
        }
    }

    // Interceptamos la acción que ocurre cuando pulsas el botón
    void onLike(cocos2d::CCObject* sender) {
        // Envia la petición HTTP original (likeGJItem21.php) al servidor
        CommentCell::onLike(sender);
        
        // El juego nativo intentará bloquear el botón inmediatamente tras el clic.
        // Volvemos a forzar su activación para permitir clics infinitos consecutivos.
        if (this->m_likeBtn) {
            this->m_likeBtn->setEnabled(true);
            this->m_likeBtn->setOpacity(255);
        }
        
        log::info("Petición de interacción enviada sin restricciones locales.");
    }
};
